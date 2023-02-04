#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructSymbolBBox.hpp"


void StructERCSymbol::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::ERCSymbol, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    StructSthInPages0::read_raw(aVersion, localFutureLst);

    // @todo readStructure();
    // @todo not sure if this belongs into this structure and how do we know whether it
    //       is used or not? (BBox should be optional according to XSD)
    //       Probably defined by prefix?
    // readPreamble();

    // StructSymbolBBox bbox{mCtx};
    // bbox.read();
    // this->symbolBBox = bbox;

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}