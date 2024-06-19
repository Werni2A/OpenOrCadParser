#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructSthInPages0.hpp"


// @todo Probably a wrapper for Inst (Instances)
void StructSthInPages0::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::SthInPages0, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    read_raw(aVersion, localFutureLst);

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}


void StructSthInPages0::read_raw(FileFormatVersion /* aVersion */, FutureDataLst& aLocalFutureLst)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("name = {}", name);

    sourceLibrary = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("sourceLibrary = {}", sourceLibrary);

    aLocalFutureLst.checkpoint();

    color = ToColor(ds.readUint32());

    mCtx.mLogger.trace("color = {}", ::to_string(color));

    const uint16_t lenPrimitives = ds.readUint16();

    mCtx.mLogger.trace("lenPrimitives = {}", lenPrimitives);

    for(size_t i = 0u; i < lenPrimitives; ++i)
    {
        bool hasAdditionalBytes = false;

        if(i + 1 < lenPrimitives)
        {
            const auto currOffset = ds.getCurrentOffset();
            try
            {
                const Primitive primitive = parser.readPrefixPrimitive();
                parser.readPrimitive(primitive);
                // Here in between could be additional data.
                // Check if this is the case
                parser.readPrefixPrimitive();
            }
            catch(...)
            {
                hasAdditionalBytes = true;
            }
            ds.setCurrentOffset(currOffset);
        }

        const Primitive primitive = parser.readPrefixPrimitive();

        parser.readPrimitive(primitive);

        if(hasAdditionalBytes)
        {
            ds.printUnknownData(8U, "Seems like all zero");
        }
    }

    // @todo Looks like it has one of {0, 8, 16 , 20} Byte in size
    //       16 Byte could be the coordinates with 4 byte each value
    //       20 Byte could be the 4 Byte coordinates with with some additional value
    mCtx.mLogger.trace("Calculating {} - {} == 8", aLocalFutureLst.cbegin()->getStopOffset(), ds.getCurrentOffset());
    if(aLocalFutureLst.getNextCheckpointPos().value_or(0U) - ds.getCurrentOffset() == std::size_t{8U})
    {
        mCtx.mLogger.trace("Probably coordinates");

        const int16_t x1 = ds.readInt16();
        mCtx.mLogger.trace("x1 = {}", x1);

        const int16_t y1 = ds.readInt16();
        mCtx.mLogger.trace("y1 = {}", y1);

        const int16_t x2 = ds.readInt16();
        mCtx.mLogger.trace("x2 = {}", x2);

        const int16_t y2 = ds.readInt16();
        mCtx.mLogger.trace("y2 = {}", y2);
    }
    else
    {
        aLocalFutureLst.readUntilNextFutureData("See FuturData of StructSthInPages0 - raw");
    }

    aLocalFutureLst.checkpoint();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
}