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

#include "CommonBase.hpp"
#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "Library.hpp"
#include "Primitives/Point.hpp"
#include "Primitives/PrimArc.hpp"
#include "Primitives/PrimBezier.hpp"
#include "Primitives/PrimBitmap.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Primitives/PrimEllipse.hpp"
#include "Primitives/PrimLine.hpp"
#include "Primitives/PrimPolygon.hpp"
#include "Primitives/PrimPolyline.hpp"
#include "Primitives/PrimRect.hpp"
#include "Primitives/PrimSymbolVector.hpp"
#include "Structures/StructAlias.hpp"
#include "Structures/StructBusEntry.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructGeneralProperties.hpp"
#include "Structures/StructGlobal.hpp"
#include "Structures/StructGlobalSymbol.hpp"
#include "Structures/StructGraphicBoxInst.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"
#include "Structures/StructGraphicLineInst.hpp"
#include "Structures/StructHierarchicSymbol.hpp"
#include "Structures/StructOffPageSymbol.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructPinIdxMapping.hpp"
#include "Structures/StructPinShapeSymbol.hpp"
#include "Structures/StructPort.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/StructSthInHierarchy1.hpp"
#include "Structures/StructSthInHierarchy2.hpp"
#include "Structures/StructSthInPages0.hpp"
#include "Structures/StructSymbolBBox.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "Structures/StructT0x10.hpp"
#include "Structures/StructT0x1f.hpp"
#include "Structures/StructT0x34.hpp"
#include "Structures/StructT0x35.hpp"
#include "Structures/StructTitleBlock.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"
#include "Structures/StructWireBus.hpp"
#include "Structures/StructWireScalar.hpp"
#include "Structures/TrailingProperties.hpp"


FutureDataLst mFutureDataLst{};


void CommonBase::discard_until_preamble()
{
    const int patternSize = 4;
    std::array<uint8_t, patternSize> buffer = {0};

    const size_t startOffset = mDs.get().getCurrentOffset();

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
        mDs.get().read(reinterpret_cast<char*>(buffer.data()) + buffer.size() - 1, 1);

        if(mDs.get().isEoF())
        {
            const std::string msg = fmt::format("{}: Unexpectedly reached end-of-file!",
                getMethodName(this, __func__));

            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
    }

    // Put back the preamble such that it can be parsed in the next step
    for(size_t i = 0u; i < preamble.size(); ++i)
    {
        mDs.get().putback(preamble[preamble.size() - 1 - i]);
    }

    const size_t endOffset = mDs.get().getCurrentOffset();

    spdlog::debug("{}: Discarded {} Byte until next preamble",
        getMethodName(this, __func__), endOffset - startOffset);
}


Structure CommonBase::auto_read_prefixes()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const size_t startOffset = mDs.get().getCurrentOffset();

    const auto logLevel = spdlog::get_level();
    spdlog::set_level(spdlog::level::off);

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
        mDs.get().clear();

        failed = false;

        try
        {
            read_prefixes(prefixCtr, true);
        }
        catch(const std::exception& e)
        {
            failed = true;
        }

        mDs.get().setCurrentOffset(startOffset);

        // Reading the prefixes might read beyond EoF in some cases,
        // because its just a prediction, we do not care. Therefore
        // reset the EoF flag.
        mDs.get().clear();

        if(!failed)
        {
            break;
        }
    }

    // Reading the prefixes might read beyond EoF in some cases,
    // because its just a prediction, we do not care. Therefore
    // reset the EoF flag.
    mDs.get().clear();

    spdlog::set_level(logLevel);

    if(failed)
    {
        const std::string msg = fmt::format("{}: Could not find valid number of prefixes!"
            " (maximum is set to {} but could be higher)",
            getMethodName(this, __func__), maxPrefixes);

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    Structure structure = read_prefixes(prefixCtr);

    // @todo Looks like each structure type has a fixed number of prefixes
    //       I.e. figure out the numbers for each structure and move the
    //       parsing code into the structure specific parser. This should
    //       get rid of auto_read_prefixes.
    spdlog::debug("{}: Found {} prefixes for {}\n",
        getMethodName(this, __func__), prefixCtr, ::to_string(structure));

    mDs.get().sanitizeNoEoF();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return structure;
}


Structure CommonBase::auto_read_prefixes(Structure aExpectedStruct)
{
    const Structure actualStruct = auto_read_prefixes();

    if(actualStruct != aExpectedStruct)
    {
        const std::string err = fmt::format("{}: Expected {} but got {}",
            getMethodName(this, __func__), ::to_string(aExpectedStruct), ::to_string(actualStruct));

        spdlog::error(err);
        throw std::runtime_error(err);
    }

    return actualStruct;
}


Structure CommonBase::auto_read_prefixes(const std::vector<Structure>& aExpectedOneOfStruct)
{
    const Structure actualStruct = auto_read_prefixes();

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

        spdlog::error(err);
        throw std::runtime_error(err);
    }

    return actualStruct;
}


// Read number of prefixes, where the last one is a short prefix
Structure CommonBase::read_prefixes(size_t aNumber, bool aPrediction)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    if(aNumber == 0U)
    {
        throw std::invalid_argument(fmt::format("{}: aNumber = {} but must be > 0!",
            getMethodName(this, __func__), aNumber));
    }

    Structure firstStruct;

    std::vector<std::pair<size_t, size_t>> offsets; //!< First = Current offset absolute; Second = Offset relative to structure

    for(size_t i = 0U; i < aNumber; ++i)
    {
        Structure currStruct;

        const size_t preambleOffset = mDs.get().getCurrentOffset();

        if(i == aNumber - 1)
        {
            const auto currPrefix = read_single_prefix_short();
            currStruct = currPrefix.first;
        }
        else
        {
            const auto currPrefix = read_single_prefix();
            currStruct = currPrefix.first;

            offsets.push_back(std::pair<size_t, size_t>{preambleOffset, static_cast<size_t>(currPrefix.second)});
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

            spdlog::error(msg);
            throw std::runtime_error(msg);
        }
    }

    if(!aPrediction)
    {
        if(offsets.size() >= 2U)
        {
            for(size_t i = 0U; i < offsets.size() - 1U; i += 2)
            {
                const std::pair<size_t, size_t> start_pair = offsets[i + 1U];
                const std::pair<size_t, size_t> stop_pair  = offsets[i];

                const FutureData futureData = FutureData{start_pair.first, start_pair.second,
                    stop_pair.first, stop_pair.second};

                const std::optional<FutureData> existing = mFutureDataLst.getByStartOffset(futureData.getStartOffset());

                if(existing.has_value())
                {
                    if(existing.value().getStopOffset() != futureData.getStopOffset())
                    {
                        const std::string msg = fmt::format("{}: Future data at 0x{:08x} is"
                            " either {} or {} Byte long, having both lengths does not make any sense.",
                            getMethodName(this, __func__), existing.value().getByteLen(),
                            futureData.getByteLen());

                        spdlog::error(msg);
                        throw std::runtime_error(msg);
                    }
                    else
                    {
                        // This should never happen
                        spdlog::warn("{}: Future data exists already in list",
                            getMethodName(this, __func__));
                    }
                }

                mFutureDataLst.push_back(futureData);

                spdlog::debug("{}: Found future data: {}", getMethodName(this, __func__),
                    (mFutureDataLst.end() - 1)->string());
            }
        }
        else if(offsets.size() == 1U)
        {
            spdlog::debug("{}: Found single structure beginning at 0x{:08x}",
                getMethodName(this, __func__), offsets[0].second);
        }
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return firstStruct;
}

std::pair<Structure, uint32_t> CommonBase::read_single_prefix()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const Structure typeId = ToStructure(mDs.get().readUint8());

    const uint32_t byteOffset = mDs.get().readUint32();

    spdlog::debug("{:>2} = {}: Offset = {}\n", static_cast<int>(typeId), ::to_string(typeId), byteOffset);

    mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 0");
    // mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, byteOffset};
}


std::pair<Structure, uint32_t> CommonBase::read_single_prefix_short()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const Structure typeId = ToStructure(mDs.get().readUint8());

    const int16_t size = mDs.get().readInt16();

    spdlog::debug("{:>2} = {}: Size = {}\n", static_cast<int>(typeId), ::to_string(typeId), size);

    if(size >= 0)
    {
        // @todo This vector needs to be stored in the package, therefore return it to caller
        std::vector<std::pair<uint32_t, uint32_t> > nameValueMapping; //!< .first = Name Index, .second = Value Index

        for(int i = 0; i < size; ++i)
        {
            uint32_t strLstIdxName  = mDs.get().readUint32();
            uint32_t strLstIdxValue = mDs.get().readUint32();

            nameValueMapping.push_back(std::make_pair(strLstIdxName, strLstIdxValue));
        }

        for(size_t i = 0u; i < nameValueMapping.size(); ++i)
        {
            try
            {
                const auto getStr = [&, this](uint32_t idx) -> std::string
                    {
                        int64_t newIdx = static_cast<int64_t>(idx);
                        return newIdx >= 0 ? gLibrary->library->strLst.at(newIdx) : "";
                    };

                spdlog::debug("  {}: {} <- {}", i, getStr(nameValueMapping.at(i).first), getStr(nameValueMapping.at(i).second));
            }
            catch(const std::exception& e)
            {
                const std::string msg = fmt::format("{}: Tried to access strLst out of range!\n{}",
                    getMethodName(this, __func__), e.what());

                spdlog::error(msg);
                throw std::out_of_range(msg);
            }
        }
    }
    else // size < 0
    {
        // @todo Why is -1 used? The value 0 would also suffice...
        // Until now I only saw it for PinIdxMapping, Properties and SymbolDisplayProp
        spdlog::warn("{}: What does {} mean?", ::to_string(typeId), size); // @todo Figure out
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, size};
}


void CommonBase::readPreamble()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::size_t startOffset = mDs.get().getCurrentOffset();

    // Try to find prefix and read trailing data. If
    // the prefix is not there, just skip it.
    try
    {
        // Magic number indicating some data
        mDs.get().assumeData({0xff, 0xe4, 0x5c, 0x39}, getMethodName(this, __func__) + " Preamble Check Failed");

        const uint32_t dataLen = mDs.get().readUint32();
        mDs.get().printUnknownData(dataLen, fmt::format("{}: Trailing preamble data",
            getMethodName(this, __func__)));
    }
    catch(const std::runtime_error& err)
    {
        mDs.get().setCurrentOffset(startOffset);
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
}



std::optional<FutureData> CommonBase::getFutureData()
{
    const size_t startOffset = mDs.get().getCurrentOffset();

    const std::optional<FutureData> thisFuture = mFutureDataLst.getByStartOffset(startOffset);

    if(thisFuture.has_value())
    {
        spdlog::info("Found this structure in future data: 0x{:08x} -> 0x{:08x} ({} Byte)",
            thisFuture.value().getStartOffset(), thisFuture.value().getStopOffset(),
            thisFuture.value().getByteLen());
    }
    else
    {
        spdlog::warn("Did not find this structure in future data with startOffset 0x{:08x}",
            startOffset);

        constexpr std::size_t showElemCnt = 10U;
        spdlog::debug("Last {} FutureDataLst Elements:", showElemCnt);

        for(auto it = (mFutureDataLst.size() <= showElemCnt) ?
                mFutureDataLst.cbegin() :
                (mFutureDataLst.cbegin() + mFutureDataLst.size() - showElemCnt);
            it != mFutureDataLst.cend(); ++it)
        {
            spdlog::debug(it->string());
        }
    }

    return thisFuture;
}


void CommonBase::sanitizeThisFutureSize(std::optional<FutureData> aThisFuture)
{
    const size_t stopOffset = mDs.get().getCurrentOffset();

    if(aThisFuture.has_value())
    {
        if(aThisFuture.value().getStopOffset() != stopOffset)
        {
            const std::string msg = fmt::format("{}: StopOffsets differ! 0x{:08x} (expected) vs. 0x{:08x} (actual)",
                getMethodName(this, __func__), aThisFuture.value().getStopOffset(), stopOffset);

            spdlog::error(msg);
            spdlog::warn("The structure may have changed due to version differences!");
            throw std::runtime_error(msg);
        }
    }
    else
    {
        spdlog::debug("{}: Could not verify structure size as future is not available.", __func__);
    }
}


std::optional<FutureData> CommonBase::checkTrailingFuture()
{
    const size_t stopOffset = mDs.get().getCurrentOffset();

    const std::optional<FutureData> nextFuture = mFutureDataLst.getByStartOffset(stopOffset);

    if(nextFuture.has_value())
    {
        spdlog::warn("Detected trailing future data at 0x{:08x}", nextFuture.value().getStartOffset());
    }

    return nextFuture;
}


void CommonBase::readOptionalTrailingFuture()
{
    std::optional<FutureData> future = checkTrailingFuture();

    if(future.has_value())
    {
        mDs.get().printUnknownData(future.value().getByteLen(),
            fmt::format("{}: Trailing Future Data", getMethodName(this, __func__)));
    }

    sanitizeThisFutureSize(future);
}


Primitive CommonBase::readPrefixPrimitive()
{
    Primitive primitive1 = ToPrimitive(mDs.get().readUint8());
    Primitive primitive2 = ToPrimitive(mDs.get().readUint8());

    if(primitive1 != primitive2)
    {
        const std::string msg = fmt::format("{}: Primitives {} != {}",
            getMethodName(this, __func__), ::to_string(primitive1),
            ::to_string(primitive2));

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    return primitive1;
}


std::unique_ptr<PrimBase> CommonBase::readPrimitive()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    Primitive typeId = ToPrimitive(mDs.get().peek(1)[0]);

    std::unique_ptr<PrimBase> obj = readPrimitive(typeId);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return obj;
}


std::unique_ptr<PrimBase> CommonBase::readPrimitive(Primitive aPrimitive)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    std::unique_ptr<PrimBase> obj{};

    switch(aPrimitive)
    {
        case Primitive::Rect:         obj = std::make_unique<PrimRect>(mDs);         break;
        case Primitive::Line:         obj = std::make_unique<PrimLine>(mDs);         break;
        case Primitive::Arc:          obj = std::make_unique<PrimArc>(mDs);          break;
        case Primitive::Ellipse:      obj = std::make_unique<PrimEllipse>(mDs);      break;
        case Primitive::Polygon:      obj = std::make_unique<PrimPolygon>(mDs);      break;
        case Primitive::Polyline:     obj = std::make_unique<PrimPolyline>(mDs);     break;
        case Primitive::CommentText:  obj = std::make_unique<PrimCommentText>(mDs);  break;
        case Primitive::Bitmap:       obj = std::make_unique<PrimBitmap>(mDs);       break;
        case Primitive::SymbolVector: obj = std::make_unique<PrimSymbolVector>(mDs); break;
        case Primitive::Bezier:       obj = std::make_unique<PrimBezier>(mDs);       break;
        default:
            const std::string msg = fmt::format("{}: Primitive {} is not implemented!",
                getMethodName(this, __func__), ::to_string(aPrimitive));

            spdlog::error(msg);
            throw std::runtime_error(msg);
            break;
    }

    if(obj.get() != nullptr)
    {
        obj->read();
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return obj;
}


std::unique_ptr<CommonBase> CommonBase::readStructure()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    Structure typeId = ToStructure(mDs.get().peek(1)[0]);

    std::unique_ptr<CommonBase> obj = readStructure(typeId);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return obj;
}


std::unique_ptr<CommonBase> CommonBase::readStructure(Structure aStructure)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    std::unique_ptr<CommonBase> obj{};

    switch(aStructure)
    {
        case Structure::Alias:                  obj = std::make_unique<StructAlias>(mDs);                  break;
        case Structure::BusEntry:               obj = std::make_unique<StructBusEntry>(mDs);               break;
        case Structure::ERCSymbol:              obj = std::make_unique<StructERCSymbol>(mDs);              break;
        case Structure::Global:                 obj = std::make_unique<StructGlobal>(mDs);                 break;
        case Structure::GlobalSymbol:           obj = std::make_unique<StructGlobalSymbol>(mDs);           break;
        case Structure::GraphicBoxInst:         obj = std::make_unique<StructGraphicBoxInst>(mDs);         break;
        case Structure::GraphicCommentTextInst: obj = std::make_unique<StructGraphicCommentTextInst>(mDs); break;
        case Structure::GraphicLineInst:        obj = std::make_unique<StructGraphicLineInst>(mDs);        break;
        case Structure::OffPageSymbol:          obj = std::make_unique<StructOffPageSymbol>(mDs);          break;
        case Structure::PartInst:               obj = std::make_unique<StructPartInst>(mDs);               break;
        case Structure::PinIdxMapping:          obj = std::make_unique<StructPinIdxMapping>(mDs);          break;
        case Structure::PinShapeSymbol:         obj = std::make_unique<StructPinShapeSymbol>(mDs);         break;
        case Structure::Port:                   obj = std::make_unique<StructPort>(mDs);                   break;
        case Structure::PortSymbol:             obj = std::make_unique<StructHierarchicSymbol>(mDs);       break;
        case Structure::Primitives:             obj = std::make_unique<StructPrimitives>(mDs);             break;
        case Structure::Properties:             obj = std::make_unique<StructProperties>(mDs);             break;
        case Structure::SthInPages0:            obj = std::make_unique<StructSthInPages0>(mDs);            break;
        case Structure::SymbolDisplayProp:      obj = std::make_unique<StructSymbolDisplayProp>(mDs);      break;
        case Structure::SymbolPinBus:           obj = std::make_unique<StructSymbolPinBus>(mDs);           break;
        case Structure::SymbolPinScalar:        obj = std::make_unique<StructSymbolPinScalar>(mDs);        break;
        case Structure::T0x10:                  obj = std::make_unique<StructT0x10>(mDs);                  break;
        case Structure::T0x1f:                  obj = std::make_unique<StructT0x1f>(mDs);                  break;
        case Structure::T0x34:                  obj = std::make_unique<StructT0x34>(mDs);                  break;
        case Structure::T0x35:                  obj = std::make_unique<StructT0x35>(mDs);                  break;
        case Structure::TitleBlock:             obj = std::make_unique<StructTitleBlock>(mDs);             break;
        case Structure::TitleBlockSymbol:       obj = std::make_unique<StructTitleBlockSymbol>(mDs);       break;
        case Structure::WireBus:                obj = std::make_unique<StructWireBus>(mDs);                break;
        case Structure::WireScalar:             obj = std::make_unique<StructWireScalar>(mDs);             break;
        default:

            const std::optional<FutureData> futureData = getFutureData();

            const std::string msg = fmt::format("{}: Structure {} is not implemented!",
                getMethodName(this, __func__), ::to_string(aStructure));

            if(futureData.has_value())
            {
                mDs.get().printUnknownData(futureData.value().getByteLen(), msg);
            }
            else
            {
                spdlog::error(msg);
                throw std::runtime_error(msg);
            }

            break;
    }

    if(obj.get() != nullptr)
    {
        obj->read();
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    return obj;
}


void CommonBase::checkInterpretedDataLen(const std::string& aFuncName, size_t aStartOffset, size_t aEndOffset, size_t aExpectedLen)
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


FileFormatVersion CommonBase::predictVersion()
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;

    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

    const size_t initial_offset = mDs.get().getCurrentOffset();

    // Testing different versions on a try and error basis
    // should not write into log files
    const auto logLevel = spdlog::get_level();
    spdlog::set_level(spdlog::level::off);

    for(const auto& version : versions)
    {
        bool found = true;

        try
        {
            read(version);
        }
        catch(...)
        {
            found = false;
        }

        mDs.get().setCurrentOffset(initial_offset);

        if(found)
        {
            prediction = version;
            break;
        }
    }

    // Restore user log level
    spdlog::set_level(logLevel);

    if(prediction == FileFormatVersion::Unknown)
    {
        // @todo Fix this hack
        // Set to previous default value
        // s.t. tests not fail
        prediction = FileFormatVersion::C;
    }

    return prediction;
}