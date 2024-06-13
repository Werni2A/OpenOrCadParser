#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include <fmt/color.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "Database.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "GenericParser.hpp"
#include "Record.hpp"
#include "RecordFactory.hpp"
#include "Stream.hpp"
#include "Streams/StreamLibrary.hpp"

// class StreamLibrary;


void GenericParser::discard_until_preamble()
{
    const int patternSize = 4;
    std::array<uint8_t, patternSize> buffer = {0};

    const size_t startOffset = mCtx.mDs.getCurrentOffset();

    // Magic number specifying the beginning of a struct
    const std::array<uint8_t, patternSize> preamble = {0xff, 0xe4, 0x5c, 0x39};

    auto shift_left = [](std::array<uint8_t, patternSize>& buffer)
    {
        for(size_t i = 0u; i < buffer.size() - 1; ++i)
        {
            buffer[i] = buffer[i + 1];
        }
    };

    while(buffer != preamble)
    {
        shift_left(buffer);
        mCtx.mDs.read(reinterpret_cast<char*>(buffer.data()) + buffer.size() - 1, 1);

        if(mCtx.mDs.isEoF())
        {
            const std::string msg = fmt::format("{}: Unexpectedly reached end-of-file!",
                getMethodName(this, __func__));

            mCtx.mLogger.debug(msg);
            throw std::runtime_error(msg);
        }
    }

    // Put back the preamble such that it can be parsed in the next step
    for(size_t i = 0u; i < preamble.size(); ++i)
    {
        mCtx.mDs.putback(preamble[preamble.size() - 1 - i]);
    }

    const size_t endOffset = mCtx.mDs.getCurrentOffset();
    const size_t discardedByte = endOffset - startOffset;

    mCtx.mLogger.debug("{}: Discarded {} Byte until next preamble",
        getMethodName(this, __func__), discardedByte);

    mCtx.mDs.setCurrentOffset(startOffset);
    mCtx.mDs.printUnknownData(discardedByte, "Discarded Bytes");
}


Structure GenericParser::auto_read_prefixes(FutureDataLst& aFutureDataLst)
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    const size_t startOffset = mCtx.mDs.getCurrentOffset();

    mCtx.mLogger.set_level(spdlog::level::off);

    bool failed = true;
    size_t prefixCtr = 0U;

    // Count from back to make it easier catching long preambles
    // that could succeed by mistake if the the algorithm detects
    // no errors in a shorter version.
    const size_t maxPrefixes = 10U;
    for(prefixCtr = maxPrefixes; prefixCtr >= 1U; --prefixCtr)
    {
        // Reading the prefixes might read beyond EoF in some cases,
        // because its just a prediction, we do not care. Therefore
        // reset the EoF flag.
        mCtx.mDs.clear();

        failed = false;

        try
        {
            FutureDataLst tmpLst{mCtx};
            read_prefixes(prefixCtr, tmpLst);
        }
        catch(const std::exception& e)
        {
            failed = true;
        }

        mCtx.mDs.setCurrentOffset(startOffset);

        // Reading the prefixes might read beyond EoF in some cases,
        // because its just a prediction, we do not care. Therefore
        // reset the EoF flag.
        mCtx.mDs.clear();

        if(!failed)
        {
            break;
        }
    }

    // Reading the prefixes might read beyond EoF in some cases,
    // because its just a prediction, we do not care. Therefore
    // reset the EoF flag.
    mCtx.mDs.clear();

    mCtx.mLogger.set_level(mCtx.mLogLevel);

    if(failed)
    {
        const std::string msg = fmt::format("{}: Could not find valid number of prefixes!"
            " (maximum is set to {} but could be higher)",
            getMethodName(this, __func__), maxPrefixes);

        mCtx.mLogger.debug(msg);
        throw std::runtime_error(msg);
    }

    const Structure structure = read_prefixes(prefixCtr, aFutureDataLst);

    // @todo Looks like each structure type has a fixed number of prefixes
    //       I.e. figure out the numbers for each structure and move the
    //       parsing code into the structure specific parser. This should
    //       get rid of auto_read_prefixes.
    mCtx.mLogger.debug("{}: Found {} prefixes for {}\n",
        getMethodName(this, __func__), prefixCtr, ::to_string(structure));
    mCtx.mLogger.debug("    {}", aFutureDataLst.string());

    mCtx.mDs.sanitizeNoEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return structure;
}


Structure GenericParser::auto_read_prefixes(Structure aExpectedStruct, FutureDataLst& aFutureDataLst)
{
    const Structure actualStruct = auto_read_prefixes(aFutureDataLst);

    if(actualStruct != aExpectedStruct)
    {
        const std::string err = fmt::format("{}: Expected {} but got {}",
            getMethodName(this, __func__), ::to_string(aExpectedStruct), ::to_string(actualStruct));

        mCtx.mLogger.debug(err);
        throw std::runtime_error(err);
    }

    return actualStruct;
}


Structure GenericParser::auto_read_prefixes(const std::vector<Structure>& aExpectedOneOfStruct, FutureDataLst& aFutureDataLst)
{
    const Structure actualStruct = auto_read_prefixes(aFutureDataLst);

    const bool foundStructure = std::find(aExpectedOneOfStruct.cbegin(),
        aExpectedOneOfStruct.cend(), actualStruct) != aExpectedOneOfStruct.cend();

    if(!foundStructure)
    {
        std::vector<std::string> expectedStrStructs{};
        std::transform(aExpectedOneOfStruct.cbegin(), aExpectedOneOfStruct.cend(),
            expectedStrStructs.begin(), [](Structure a){ return ::to_string(a); });

        const std::string err = fmt::format("{}: Expected one of [{}] but got {}", getMethodName(this, __func__),
            std::accumulate(expectedStrStructs.cbegin(), expectedStrStructs.cend(), std::string{", "}),
            ::to_string(actualStruct));

        mCtx.mLogger.debug(err);
        throw std::runtime_error(err);
    }

    return actualStruct;
}


// Read number of prefixes, where the last one is a short prefix
Structure GenericParser::read_prefixes(size_t aNumber, FutureDataLst& aFutureDataLst)
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    if(aNumber == 0U)
    {
        throw std::invalid_argument(fmt::format("{}: aNumber = {} but must be > 0!",
            getMethodName(this, __func__), aNumber));
    }

    Structure firstStruct;

    for(size_t i = 0U; i < aNumber; ++i)
    {
        Structure currStruct;

        const size_t preambleOffset = mCtx.mDs.getCurrentOffset();

        if(i == aNumber - 1)
        {
            const auto currPrefix = read_single_prefix_short();
            currStruct = currPrefix.first;
        }
        else
        {
            const auto currPrefix = read_single_prefix();
            currStruct = currPrefix.first;
            const size_t currStructSize = static_cast<size_t>(currPrefix.second);

            const FutureData futureData{preambleOffset, currStructSize};

            aFutureDataLst.push_back(futureData);
        }

        if(i == 0U)
        {
            firstStruct = currStruct;
        }

        if(currStruct != firstStruct)
        {
            const std::string msg = fmt::format("{}: {} != {}",
                getMethodName(this, __func__), ::to_string(currStruct),
                ::to_string(firstStruct));

            mCtx.mLogger.debug(msg);
            throw std::runtime_error(msg);
        }
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return firstStruct;
}

std::pair<Structure, uint32_t> GenericParser::read_single_prefix()
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    const Structure typeId = ToStructure(mCtx.mDs.readUint8());

    const uint32_t byteOffset = mCtx.mDs.readUint32();

    mCtx.mLogger.debug("{:>2} = {}: Offset = {}\n", static_cast<int>(typeId), ::to_string(typeId), byteOffset);

    mCtx.mDs.printUnknownData(4, getMethodName(this, __func__) + ": 0");
    // assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, byteOffset};
}


std::pair<Structure, uint32_t> GenericParser::read_single_prefix_short()
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    const Structure typeId = ToStructure(mCtx.mDs.readUint8());

    const int16_t size = mCtx.mDs.readInt16();

    mCtx.mLogger.debug("{:>2} = {}: Size = {}\n", static_cast<int>(typeId), ::to_string(typeId), size);

    if(size >= 0)
    {
        // @todo This vector needs to be stored in the package, therefore return it to caller
        std::vector<std::pair<uint32_t, uint32_t> > nameValueMapping; //!< .first = Name Index, .second = Value Index

        for(int i = 0; i < size; ++i)
        {
            uint32_t strLstIdxName  = mCtx.mDs.readUint32();
            uint32_t strLstIdxValue = mCtx.mDs.readUint32();

            nameValueMapping.push_back(std::make_pair(strLstIdxName, strLstIdxValue));
        }

        for(size_t i = 0u; i < nameValueMapping.size(); ++i)
        {
            // try
            // {
            //     const auto getStr = [&, this](uint32_t idx) -> std::string
            //         {
            //             // int64_t newIdx = static_cast<int64_t>(idx);

            //             // if(mCtx.mDb.getLibrary().has_value())
            //             // {
            //             //     const StreamLibrary& lib = dynamic_cast<StreamLibrary&>(*mCtx.mDb.getLibrary().value());
            //             //     return newIdx >= 0 ? lib.strLst.at(newIdx) : "";
            //             // }

            //             return "";
            //         };

            //     mCtx.mLogger.debug("  {}: {} <- {}", i, getStr(nameValueMapping.at(i).first), getStr(nameValueMapping.at(i).second));
            // }
            // catch(const std::exception& e)
            // {
            //     const std::string msg = fmt::format("{}: Tried to access strLst out of range!\n{}",
            //         getMethodName(this, __func__), e.what());

            //     mCtx.mLogger.debug(msg);
            //     throw std::out_of_range(msg);
            // }
        }
    }
    else // size < 0
    {
        // @todo Why is -1 used? The value 0 would also suffice...
        // Until now I only saw it for Device, PartCell and SymbolDisplayProp
        mCtx.mLogger.debug("{}: What does {} mean?", ::to_string(typeId), size); // @todo Figure out
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, size};
}


void GenericParser::readPreamble()
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    const std::size_t startOffset = mCtx.mDs.getCurrentOffset();

    // Try to find prefix and read trailing data. If
    // the prefix is not there, just skip it.
    try
    {
        // Magic number indicating some data
        mCtx.mDs.assumeData({0xff, 0xe4, 0x5c, 0x39}, getMethodName(this, __func__) + " Preamble Check Failed");

        const uint32_t dataLen = mCtx.mDs.readUint32();
        mCtx.mDs.printUnknownData(dataLen, fmt::format("{}: Trailing preamble data",
            getMethodName(this, __func__)));

        // mCtx.mLogger.debug("{}: Found preamble", getMethodName(this, __func__));
    }
    catch(const std::runtime_error& err)
    {
        mCtx.mDs.setCurrentOffset(startOffset);

        mCtx.mLogger.debug("{}: Skipping preamble", getMethodName(this, __func__));
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));
}


Primitive GenericParser::readPrefixPrimitive()
{
    Primitive primitive1 = ToPrimitive(mCtx.mDs.readUint8());
    Primitive primitive2 = ToPrimitive(mCtx.mDs.readUint8());

    if(primitive1 != primitive2)
    {
        const std::string msg = fmt::format("{}: Primitives {} != {}",
            getMethodName(this, __func__), ::to_string(primitive1),
            ::to_string(primitive2));

        mCtx.mLogger.debug(msg);
        throw std::runtime_error(msg);
    }

    return primitive1;
}


std::unique_ptr<PrimBase> GenericParser::readPrimitive()
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    Primitive typeId = ToPrimitive(mCtx.mDs.peek(1)[0]);

    std::unique_ptr<PrimBase> obj = readPrimitive(typeId);

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return obj;
}


std::unique_ptr<PrimBase> GenericParser::readPrimitive(Primitive aPrimitive)
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    const size_t startOffset = mCtx.mDs.getCurrentOffset();

    std::unique_ptr<PrimBase> obj = RecordFactory::build(mCtx, aPrimitive);

    if(obj)
    {
        try
        {
            obj->read();
        }
        catch(...)
        {
            if(mCtx.mCfg.mSkipInvalidPrim)
            {
                mCtx.mLogger.debug("{}: Skipping invalid Primitive {}",
                    getMethodName(this, __func__), ::to_string(aPrimitive));

                // Reset file position to the state before
                // the structure was parsed and failed
                mCtx.mDs.setCurrentOffset(startOffset);

                // Previous read operations might have read beyond EoF,
                // therefore reset the EoF flag.
                mCtx.mDs.clear();

                const uint32_t byteLength = mCtx.mDs.readUint32();

                mCtx.mDs.printUnknownData(byteLength - sizeof(byteLength),
                    fmt::format("{} data", ::to_string(aPrimitive)));

                readPreamble();
            }
            else
            {
                throw;
            }
        }
    }
    else
    {
        const std::string msg = fmt::format("{}: Primitive {} is not implemented!",
            getMethodName(this, __func__), ::to_string(aPrimitive));

        mCtx.mLogger.debug(msg);

        if(!mCtx.mCfg.mSkipUnknownPrim)
        {
            throw std::runtime_error(msg);
        }

        mCtx.mLogger.debug("{}: Skipping unimplemented Primitive {}",
            getMethodName(this, __func__), ::to_string(aPrimitive));

        const uint32_t byteLength = mCtx.mDs.readUint32();

        mCtx.mDs.printUnknownData(byteLength - sizeof(byteLength),
            fmt::format("{} data", ::to_string(aPrimitive)));

        readPreamble();
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return obj;
}


std::unique_ptr<Record> GenericParser::readStructure()
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    Structure typeId = ToStructure(mCtx.mDs.peek(1)[0]);

    std::unique_ptr<Record> obj = readStructure(typeId);

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return obj;
}


std::unique_ptr<Record> GenericParser::readStructure(Structure aStructure)
{
    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    const size_t startOffset = mCtx.mDs.getCurrentOffset();

    std::unique_ptr<Record> obj = RecordFactory::build(mCtx, aStructure);

    if(obj)
    {
        try
        {
            obj->read();
        }
        catch(...)
        {
            if(mCtx.mCfg.mSkipInvalidStruct)
            {
                mCtx.mLogger.debug("{}: Skipping invalid Structure {}",
                    getMethodName(this, __func__), ::to_string(aStructure));

                // Reset file position to the state before
                // the structure was parsed and failed
                mCtx.mDs.setCurrentOffset(startOffset);

                // Previous read operations might have read beyond EoF,
                // therefore reset the EoF flag.
                mCtx.mDs.clear();

                FutureDataLst localFutureDataLst{mCtx};

                auto_read_prefixes(localFutureDataLst);

                localFutureDataLst.readRestOfStructure();
            }
            else
            {
                throw;
            }
        }
    }
    else
    {
        const std::string msg = fmt::format("{}: Structure {} is unimplemented!",
            getMethodName(this, __func__), ::to_string(aStructure));

        mCtx.mLogger.debug(msg);

        if(!mCtx.mCfg.mSkipUnknownStruct)
        {
            throw std::runtime_error(msg);
        }

        mCtx.mLogger.debug("{}: Skipping unimplemented Structure {}",
            getMethodName(this, __func__), ::to_string(aStructure));

        FutureDataLst localFutureDataLst{mCtx};

        auto_read_prefixes(localFutureDataLst);

        localFutureDataLst.readRestOfStructure();
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), mCtx.mDs.getCurrentOffset()));

    return obj;
}


bool GenericParser::tryRead(std::function<void(void)> aFunction)
{
    const auto offsetBeforeTest = mCtx.mDs.getCurrentOffset();
    bool checkFailed = false;

    try
    {
        aFunction();
    }
    catch(...)
    {
        checkFailed = true;
    }

    mCtx.mDs.setCurrentOffset(offsetBeforeTest);

    return !checkFailed;
}


void GenericParser::checkInterpretedDataLen(const std::string& aFuncName, size_t aStartOffset, size_t aEndOffset, size_t aExpectedLen)
{
    if(aStartOffset > aEndOffset)
    {
        throw std::invalid_argument("Start offset must be smaller or equal to end offset!");
    }

    const size_t actuallLen = aEndOffset - aStartOffset;

    if(actuallLen != aExpectedLen)
    {
        throw MisinterpretedData(aFuncName.c_str(), aStartOffset, aExpectedLen, aEndOffset);
    }
}


FileFormatVersion GenericParser::predictVersion(std::function<void(FileFormatVersion)> aFunc)
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;

    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C,
        FileFormatVersion::D,
        FileFormatVersion::E,
        FileFormatVersion::F,
        FileFormatVersion::G,
        FileFormatVersion::H,
        FileFormatVersion::I,
        FileFormatVersion::J,
        FileFormatVersion::K,
        FileFormatVersion::L,
        FileFormatVersion::M,
        FileFormatVersion::N,
        FileFormatVersion::O,
        FileFormatVersion::P
    };

    const size_t initial_offset = mCtx.mDs.getCurrentOffset();

    // Testing different versions on a try and error basis
    // should not write into log files
    mCtx.mLogger.set_level(spdlog::level::off);

    for(const auto& version : versions)
    {
        bool found = true;

        try
        {
            aFunc(version);
        }
        catch(...)
        {
            found = false;
        }

        mCtx.mDs.setCurrentOffset(initial_offset);

        if(found)
        {
            prediction = version;
            break;
        }
    }

    // Restore user log level
    mCtx.mLogger.set_level(mCtx.mLogLevel);

    mCtx.mLogger.debug("{}: Predicted version to be {}",
        getMethodName(this, __func__), magic_enum::enum_name(prediction));

    if(prediction == FileFormatVersion::Unknown)
    {
        // @todo Fix this hack
        // Set to previous default value
        // s.t. tests not fail
        prediction = FileFormatVersion::C;
        mCtx.mLogger.debug("{}: Setting version to {} anyway",
            getMethodName(this, __func__), magic_enum::enum_name(prediction));
    }

    return prediction;
}