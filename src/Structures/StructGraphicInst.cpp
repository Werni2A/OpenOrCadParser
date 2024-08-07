#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructGraphicInst.hpp"

void OOCP::StructGraphicInst::read(FutureDataLst& mLocalFutureLst, FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    parser.readPreamble();

    mLocalFutureLst.checkpoint();

    ds.printUnknownData(8, getMethodName(this, __func__) + ": 0");

    name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("name = {}", name);

    dbId = ds.readUint32();

    mCtx.mLogger.trace("dbId = {}", dbId);

    locY = ds.readInt16();
    locX = ds.readInt16();

    mCtx.mLogger.trace("locX = {}", locX);
    mCtx.mLogger.trace("locY = {}", locY);

    y2 = ds.readInt16();
    x2 = ds.readInt16();

    mCtx.mLogger.trace("y2 = {}", y2);
    mCtx.mLogger.trace("x2 = {}", x2);

    x1 = ds.readInt16();
    y1 = ds.readInt16();

    mCtx.mLogger.trace("x1 = {}", x1);
    mCtx.mLogger.trace("y1 = {}", y1);

    color = ToColor(ds.readUint8());

    mCtx.mLogger.trace("color = {}", OOCP::to_string(color));

    ds.printUnknownData(1, getMethodName(this, __func__) + ": StructGraphicInst - 1");
    ds.printUnknownData(1,
        getMethodName(this, __func__) + ": StructGraphicInst - 2"); // @todo Probably structure ID of current structure
    ds.printUnknownData(1, getMethodName(this, __func__) + ": StructGraphicInst - 3");

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    mCtx.mLogger.trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(parser.readStructure()));
    }

    // @todo This flag relates to Enums/Structure
    //       Looks like it points from the Instance ID
    //       to the actual structure ID.
    //       E.g. StructGlobal -> StructGlobalSymbol
    //            StructPort   -> StructPortSymbol
    const uint8_t unknownFlag = ds.readUint8();

    mCtx.mLogger.trace("unknownFlag = {}", unknownFlag);

    // @todo Add some sanity check
    switch(unknownFlag)
    {
        // StructGraphicBoxInst
        // StructGraphicCommentTextInst
        // StructGraphicLineInst
        case 0x02:
            sthInPages0 = dynamic_pointer_cast<StructSthInPages0>(parser.readStructure());
            break;

        case 0x21: // StructGlobal
        case 0x22: // StructPort
        case 0x23: // OffPageConnector
        case 0x40: // TitleBlock
        case 0x4b: // StructERCObject
            // Do nothing
            break;

        default:
            throw std::runtime_error(fmt::format("Unexpected value {} for unknownFlag", unknownFlag));
            break;
    }

    mLocalFutureLst.checkpoint();
}