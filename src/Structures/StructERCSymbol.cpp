#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructSymbolBBox.hpp"


void StructERCSymbol::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::ERCSymbol, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    StructSthInPages0::read_raw(aVersion, localFutureLst);

    // @todo Move into read_raw above?
    // @todo readStructure();
    // @todo not sure if this belongs into this structure and how do we know whether it
    //       is used or not? (BBox should be optional according to XSD)
    //       Probably defined by prefix?
    // ds.readPreamble();

    // StructSymbolBBox bbox{mCtx};
    // bbox.read();
    // this->symbolBBox = bbox;

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 0");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}