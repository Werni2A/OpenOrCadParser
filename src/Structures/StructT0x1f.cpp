#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructT0x1f.hpp"


// @todo Probably specifies the 'Package Properties'
void StructT0x1f::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::T0x1f, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    std::string unknownStr0 = ds.readStringLenZeroTerm(); // @todo figure out
    spdlog::trace("{}: unknownStr0 = {}", __func__, unknownStr0);

    localFutureLst.checkpoint();

    refDes = ds.readStringLenZeroTerm();

    spdlog::trace("refDes = {}", refDes);

    std::string unknownStr1 = ds.readStringLenZeroTerm(); // @todo figure out
    spdlog::trace("{}: unknownStr1 = {}", __func__, unknownStr1);

    pcbFootprint = ds.readStringLenZeroTerm();

    const uint16_t lenPinIdxMappings = ds.readUint16();

    spdlog::trace("lenPinIdxMappings = {}", lenPinIdxMappings);

    for(size_t i = 0u; i < lenPinIdxMappings; ++i)
    {
        pinIdxMappings.push_back(dynamic_pointer_cast<StructPinIdxMapping>(parser.readStructure()));
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}