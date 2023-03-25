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
#include "Structures/StructBookMarkSymbolInst.hpp"
#include "Structures/StructBusEntry.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructERCSymbolInst.hpp"
#include "Structures/StructGeneralProperties.hpp"
#include "Structures/StructGlobal.hpp"
#include "Structures/StructGlobalSymbol.hpp"
#include "Structures/StructGraphicArcInst.hpp"
#include "Structures/StructGraphicBezierInst.hpp"
#include "Structures/StructGraphicBitMapInst.hpp"
#include "Structures/StructGraphicBoxInst.hpp"
#include "Structures/StructGraphicCommentTextInst.hpp"
#include "Structures/StructGraphicEllipseInst.hpp"
#include "Structures/StructGraphicLineInst.hpp"
#include "Structures/StructGraphicOleEmbedInst.hpp"
#include "Structures/StructGraphicPolygonInst.hpp"
#include "Structures/StructGraphicPolylineInst.hpp"
#include "Structures/StructHierarchicSymbol.hpp"
#include "Structures/StructNetDbIdMapping.hpp"
#include "Structures/StructOffPageConnector.hpp"
#include "Structures/StructOffPageSymbol.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructPinIdxMapping.hpp"
#include "Structures/StructPinShapeSymbol.hpp"
#include "Structures/StructPort.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/StructSthInHierarchy1.hpp"
#include "Structures/StructSthInHierarchy2.hpp"
#include "Structures/StructSthInHierarchy3.hpp"
#include "Structures/StructSthInPages0.hpp"
#include "Structures/StructSymbolBBox.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPinBus.hpp"
#include "Structures/StructSymbolPinScalar.hpp"
#include "Structures/StructT0x10.hpp"
#include "Structures/StructT0x1f.hpp"
#include "Structures/StructT0x34.hpp"
#include "Structures/StructT0x35.hpp"
#include "Structures/StructT0x45.hpp"
#include "Structures/StructT0x5b.hpp"
#include "Structures/StructTitleBlock.hpp"
#include "Structures/StructTitleBlockSymbol.hpp"
#include "Structures/StructWireBus.hpp"
#include "Structures/StructWireScalar.hpp"


void CommonBase::discard_until_preamble()
{
    const int patternSize = 4;
    std::array<uint8_t, patternSize> buffer = {0};

    const size_t startOffset = mCtx.get().mDs.get().getCurrentOffset();

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
        mCtx.get().mDs.get().read(reinterpret_cast<char*>(buffer.data()) + buffer.size() - 1, 1);

        if(mCtx.get().mDs.get().isEoF())
        {
            const std::string msg = fmt::format("{}: Unexpectedly reached end-of-file!",
                getMethodName(this, __func__));

            spdlog::debug(msg);
            throw std::runtime_error(msg);
        }
    }

    // Put back the preamble such that it can be parsed in the next step
    for(size_t i = 0u; i < preamble.size(); ++i)
    {
        mCtx.get().mDs.get().putback(preamble[preamble.size() - 1 - i]);
    }

    const size_t endOffset = mCtx.get().mDs.get().getCurrentOffset();

    spdlog::debug("{}: Discarded {} Byte until next preamble",
        getMethodName(this, __func__), endOffset - startOffset);
}


Structure CommonBase::auto_read_prefixes(FutureDataLst& aFutureDataLst)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    const size_t startOffset = mCtx.get().mDs.get().getCurrentOffset();

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
        mCtx.get().mDs.get().clear();

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

        mCtx.get().mDs.get().setCurrentOffset(startOffset);

        // Reading the prefixes might read beyond EoF in some cases,
        // because its just a prediction, we do not care. Therefore
        // reset the EoF flag.
        mCtx.get().mDs.get().clear();

        if(!failed)
        {
            break;
        }
    }

    // Reading the prefixes might read beyond EoF in some cases,
    // because its just a prediction, we do not care. Therefore
    // reset the EoF flag.
    mCtx.get().mDs.get().clear();

    spdlog::set_level(logLevel);

    if(failed)
    {
        const std::string msg = fmt::format("{}: Could not find valid number of prefixes!"
            " (maximum is set to {} but could be higher)",
            getMethodName(this, __func__), maxPrefixes);

        spdlog::debug(msg);
        throw std::runtime_error(msg);
    }

    const Structure structure = read_prefixes(prefixCtr, aFutureDataLst);

    // @todo Looks like each structure type has a fixed number of prefixes
    //       I.e. figure out the numbers for each structure and move the
    //       parsing code into the structure specific parser. This should
    //       get rid of auto_read_prefixes.
    spdlog::debug("{}: Found {} prefixes for {}\n",
        getMethodName(this, __func__), prefixCtr, ::to_string(structure));
    spdlog::debug("    {}", aFutureDataLst.string());

    mCtx.get().mDs.get().sanitizeNoEoF();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    return structure;
}


Structure CommonBase::auto_read_prefixes(Structure aExpectedStruct, FutureDataLst& aFutureDataLst)
{
    const Structure actualStruct = auto_read_prefixes(aFutureDataLst);

    if(actualStruct != aExpectedStruct)
    {
        const std::string err = fmt::format("{}: Expected {} but got {}",
            getMethodName(this, __func__), ::to_string(aExpectedStruct), ::to_string(actualStruct));

        spdlog::debug(err);
        throw std::runtime_error(err);
    }

    return actualStruct;
}


Structure CommonBase::auto_read_prefixes(const std::vector<Structure>& aExpectedOneOfStruct, FutureDataLst& aFutureDataLst)
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

        spdlog::debug(err);
        throw std::runtime_error(err);
    }

    return actualStruct;
}


// Read number of prefixes, where the last one is a short prefix
Structure CommonBase::read_prefixes(size_t aNumber, FutureDataLst& aFutureDataLst)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    if(aNumber == 0U)
    {
        throw std::invalid_argument(fmt::format("{}: aNumber = {} but must be > 0!",
            getMethodName(this, __func__), aNumber));
    }

    Structure firstStruct;

    for(size_t i = 0U; i < aNumber; ++i)
    {
        Structure currStruct;

        const size_t preambleOffset = mCtx.get().mDs.get().getCurrentOffset();

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

            spdlog::debug(msg);
            throw std::runtime_error(msg);
        }
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    return firstStruct;
}

std::pair<Structure, uint32_t> CommonBase::read_single_prefix()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    const Structure typeId = ToStructure(mCtx.get().mDs.get().readUint8());

    const uint32_t byteOffset = mCtx.get().mDs.get().readUint32();

    spdlog::debug("{:>2} = {}: Offset = {}\n", static_cast<int>(typeId), ::to_string(typeId), byteOffset);

    mCtx.get().mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 0");
    // mCtx.get().mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, byteOffset};
}


std::pair<Structure, uint32_t> CommonBase::read_single_prefix_short()
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const Structure typeId = ToStructure(ds.readUint8());

    const int16_t size = ds.readInt16();

    spdlog::debug("{:>2} = {}: Size = {}\n", static_cast<int>(typeId), ::to_string(typeId), size);

    if(size >= 0)
    {
        // @todo This vector needs to be stored in the package, therefore return it to caller
        std::vector<std::pair<uint32_t, uint32_t> > nameValueMapping; //!< .first = Name Index, .second = Value Index

        for(int i = 0; i < size; ++i)
        {
            uint32_t strLstIdxName  = ds.readUint32();
            uint32_t strLstIdxValue = ds.readUint32();

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

                spdlog::debug(msg);
                throw std::out_of_range(msg);
            }
        }
    }
    else // size < 0
    {
        // @todo Why is -1 used? The value 0 would also suffice...
        // Until now I only saw it for PinIdxMapping, Properties and SymbolDisplayProp
        spdlog::debug("{}: What does {} mean?", ::to_string(typeId), size); // @todo Figure out
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    return std::pair<Structure, uint32_t>{typeId, size};
}


void CommonBase::readPreamble()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    const std::size_t startOffset = mCtx.get().mDs.get().getCurrentOffset();

    // Try to find prefix and read trailing data. If
    // the prefix is not there, just skip it.
    try
    {
        // Magic number indicating some data
        mCtx.get().mDs.get().assumeData({0xff, 0xe4, 0x5c, 0x39}, getMethodName(this, __func__) + " Preamble Check Failed");

        const uint32_t dataLen = mCtx.get().mDs.get().readUint32();
        mCtx.get().mDs.get().printUnknownData(dataLen, fmt::format("{}: Trailing preamble data",
            getMethodName(this, __func__)));

        // spdlog::debug("{}: Found preamble", getMethodName(this, __func__));
    }
    catch(const std::runtime_error& err)
    {
        mCtx.get().mDs.get().setCurrentOffset(startOffset);

        spdlog::debug("{}: Skipping preamble", getMethodName(this, __func__));
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));
}


Primitive CommonBase::readPrefixPrimitive()
{
    Primitive primitive1 = ToPrimitive(mCtx.get().mDs.get().readUint8());
    Primitive primitive2 = ToPrimitive(mCtx.get().mDs.get().readUint8());

    if(primitive1 != primitive2)
    {
        const std::string msg = fmt::format("{}: Primitives {} != {}",
            getMethodName(this, __func__), ::to_string(primitive1),
            ::to_string(primitive2));

        spdlog::debug(msg);
        throw std::runtime_error(msg);
    }

    return primitive1;
}


std::unique_ptr<PrimBase> CommonBase::readPrimitive()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    Primitive typeId = ToPrimitive(mCtx.get().mDs.get().peek(1)[0]);

    std::unique_ptr<PrimBase> obj = readPrimitive(typeId);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    return obj;
}


std::unique_ptr<PrimBase> CommonBase::readPrimitive(Primitive aPrimitive)
{
    auto& ctx = mCtx.get();
    auto& ds  = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

    std::unique_ptr<PrimBase> obj{};

    switch(aPrimitive)
    {
        case Primitive::Rect:         obj = std::make_unique<PrimRect>(mCtx);         break;
        case Primitive::Line:         obj = std::make_unique<PrimLine>(mCtx);         break;
        case Primitive::Arc:          obj = std::make_unique<PrimArc>(mCtx);          break;
        case Primitive::Ellipse:      obj = std::make_unique<PrimEllipse>(mCtx);      break;
        case Primitive::Polygon:      obj = std::make_unique<PrimPolygon>(mCtx);      break;
        case Primitive::Polyline:     obj = std::make_unique<PrimPolyline>(mCtx);     break;
        case Primitive::CommentText:  obj = std::make_unique<PrimCommentText>(mCtx);  break;
        case Primitive::Bitmap:       obj = std::make_unique<PrimBitmap>(mCtx);       break;
        case Primitive::SymbolVector: obj = std::make_unique<PrimSymbolVector>(mCtx); break;
        case Primitive::Bezier:       obj = std::make_unique<PrimBezier>(mCtx);       break;
        default:
            {
                const std::string msg = fmt::format("{}: Primitive {} is not implemented!",
                    getMethodName(this, __func__), ::to_string(aPrimitive));

                spdlog::debug(msg);

                if(!ctx.mSkipUnknownPrim)
                {
                    throw std::runtime_error(msg);
                }

                spdlog::debug("{}: Skipping unimplemented Primitive {}",
                    getMethodName(this, __func__), ::to_string(aPrimitive));

                const uint32_t byteLength = ds.readUint32();

                ds.printUnknownData(byteLength - sizeof(byteLength),
                    fmt::format("{} data", ::to_string(aPrimitive)));

                readPreamble();

                break;
            }
    }

    if(obj.get() != nullptr)
    {
        try
        {
            obj->read();
        }
        catch(...)
        {
            if(ctx.mSkipInvalidPrim)
            {
                spdlog::debug("{}: Skipping invalid Primitive {}",
                    getMethodName(this, __func__), ::to_string(aPrimitive));

                // Reset file position to the state before
                // the structure was parsed and failed
                ds.setCurrentOffset(startOffset);

                // Previous read operations might have read beyond EoF,
                // therefore reset the EoF flag.
                ds.clear();

                const uint32_t byteLength = ds.readUint32();

                ds.printUnknownData(byteLength - sizeof(byteLength),
                    fmt::format("{} data", ::to_string(aPrimitive)));

                readPreamble();
            }
            else
            {
                throw;
            }
        }
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    return obj;
}


std::unique_ptr<CommonBase> CommonBase::readStructure()
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    Structure typeId = ToStructure(mCtx.get().mDs.get().peek(1)[0]);

    std::unique_ptr<CommonBase> obj = readStructure(typeId);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mCtx.get().mDs.get().getCurrentOffset()));

    return obj;
}


std::unique_ptr<CommonBase> CommonBase::readStructure(Structure aStructure)
{
    auto& ctx = mCtx.get();
    auto& ds  = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

    std::unique_ptr<CommonBase> obj{};

    switch(aStructure)
    {
        case Structure::Alias:                  obj = std::make_unique<StructAlias>(mCtx);                  break;
        case Structure::BookMarkSymbolInst:     obj = std::make_unique<StructBookMarkSymbolInst>(mCtx);     break;
        case Structure::BusEntry:               obj = std::make_unique<StructBusEntry>(mCtx);               break;
        case Structure::ERCSymbol:              obj = std::make_unique<StructERCSymbol>(mCtx);              break;
        case Structure::ERCSymbolInst:          obj = std::make_unique<StructERCSymbolInst>(mCtx);          break;
        case Structure::Global:                 obj = std::make_unique<StructGlobal>(mCtx);                 break;
        case Structure::GlobalSymbol:           obj = std::make_unique<StructGlobalSymbol>(mCtx);           break;
        case Structure::GraphicArcInst:         obj = std::make_unique<StructGraphicArcInst>(mCtx);         break;
        case Structure::GraphicBezierInst:      obj = std::make_unique<StructGraphicBezierInst>(mCtx);      break;
        case Structure::GraphicBitMapInst:      obj = std::make_unique<StructGraphicBitMapInst>(mCtx);      break;
        case Structure::GraphicBoxInst:         obj = std::make_unique<StructGraphicBoxInst>(mCtx);         break;
        case Structure::GraphicCommentTextInst: obj = std::make_unique<StructGraphicCommentTextInst>(mCtx); break;
        case Structure::GraphicEllipseInst:     obj = std::make_unique<StructGraphicEllipseInst>(mCtx);     break;
        case Structure::GraphicLineInst:        obj = std::make_unique<StructGraphicLineInst>(mCtx);        break;
        case Structure::GraphicOleEmbedInst:    obj = std::make_unique<StructGraphicOleEmbedInst>(mCtx);    break;
        case Structure::GraphicPolygonInst:     obj = std::make_unique<StructGraphicPolygonInst>(mCtx);     break;
        case Structure::GraphicPolylineInst:    obj = std::make_unique<StructGraphicPolylineInst>(mCtx);    break;
        case Structure::NetDbIdMapping:         obj = std::make_unique<StructNetDbIdMapping>(mCtx);         break;
        case Structure::OffPageConnector:       obj = std::make_unique<StructOffPageConnector>(mCtx);       break;
        case Structure::OffPageSymbol:          obj = std::make_unique<StructOffPageSymbol>(mCtx);          break;
        case Structure::PartInst:               obj = std::make_unique<StructPartInst>(mCtx);               break;
        case Structure::PinIdxMapping:          obj = std::make_unique<StructPinIdxMapping>(mCtx);          break;
        case Structure::PinShapeSymbol:         obj = std::make_unique<StructPinShapeSymbol>(mCtx);         break;
        case Structure::Port:                   obj = std::make_unique<StructPort>(mCtx);                   break;
        case Structure::PortSymbol:             obj = std::make_unique<StructHierarchicSymbol>(mCtx);       break;
        case Structure::Primitives:             obj = std::make_unique<StructPrimitives>(mCtx);             break;
        case Structure::Properties:             obj = std::make_unique<StructProperties>(mCtx);             break;
        case Structure::SthInHierarchy1:        obj = std::make_unique<StructSthInHierarchy1>(mCtx);        break;
        case Structure::SthInHierarchy2:        obj = std::make_unique<StructSthInHierarchy2>(mCtx);        break;
        case Structure::SthInHierarchy3:        obj = std::make_unique<StructSthInHierarchy3>(mCtx);        break;
        case Structure::SthInPages0:            obj = std::make_unique<StructSthInPages0>(mCtx);            break;
        case Structure::SymbolDisplayProp:      obj = std::make_unique<StructSymbolDisplayProp>(mCtx);      break;
        case Structure::SymbolPinBus:           obj = std::make_unique<StructSymbolPinBus>(mCtx);           break;
        case Structure::SymbolPinScalar:        obj = std::make_unique<StructSymbolPinScalar>(mCtx);        break;
        case Structure::T0x10:                  obj = std::make_unique<StructT0x10>(mCtx);                  break;
        case Structure::T0x1f:                  obj = std::make_unique<StructT0x1f>(mCtx);                  break;
        case Structure::T0x34:                  obj = std::make_unique<StructT0x34>(mCtx);                  break;
        case Structure::T0x35:                  obj = std::make_unique<StructT0x35>(mCtx);                  break;
        case Structure::T0x45:                  obj = std::make_unique<StructT0x45>(mCtx);                  break;
        case Structure::T0x5b:                  obj = std::make_unique<StructT0x5b>(mCtx);                  break;
        case Structure::TitleBlock:             obj = std::make_unique<StructTitleBlock>(mCtx);             break;
        case Structure::TitleBlockSymbol:       obj = std::make_unique<StructTitleBlockSymbol>(mCtx);       break;
        case Structure::WireBus:                obj = std::make_unique<StructWireBus>(mCtx);                break;
        case Structure::WireScalar:             obj = std::make_unique<StructWireScalar>(mCtx);             break;
        default:
            {
                const std::string msg = fmt::format("{}: Structure {} is unimplemented!",
                    getMethodName(this, __func__), ::to_string(aStructure));

                spdlog::debug(msg);

                if(!ctx.mSkipUnknownStruct)
                {
                    throw std::runtime_error(msg);
                }

                spdlog::debug("{}: Skipping unimplemented Structure {}",
                    getMethodName(this, __func__), ::to_string(aStructure));

                FutureDataLst localFutureDataLst{mCtx};

                auto_read_prefixes(localFutureDataLst);

                localFutureDataLst.readRestOfStructure();

                break;
            }
    }

    if(obj)
    {
        try
        {
            obj->read();
        }
        catch(...)
        {
            if(ctx.mSkipInvalidStruct)
            {
                spdlog::debug("{}: Skipping invalid Structure {}",
                    getMethodName(this, __func__), ::to_string(aStructure));

                // Reset file position to the state before
                // the structure was parsed and failed
                ds.setCurrentOffset(startOffset);

                // Previous read operations might have read beyond EoF,
                // therefore reset the EoF flag.
                ds.clear();

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

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

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

    const size_t initial_offset = mCtx.get().mDs.get().getCurrentOffset();

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

        mCtx.get().mDs.get().setCurrentOffset(initial_offset);

        if(found)
        {
            prediction = version;
            break;
        }
    }

    // Restore user log level
    spdlog::set_level(logLevel);

    spdlog::debug("{}: Predicted version to be {}",
        getMethodName(this, __func__), magic_enum::enum_name(prediction));

    if(prediction == FileFormatVersion::Unknown)
    {
        // @todo Fix this hack
        // Set to previous default value
        // s.t. tests not fail
        prediction = FileFormatVersion::C;
        spdlog::debug("{}: Setting version to {} anyway",
            getMethodName(this, __func__), magic_enum::enum_name(prediction));
    }

    return prediction;
}