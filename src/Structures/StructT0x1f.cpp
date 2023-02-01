#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructT0x1f.hpp"


// @todo Probably specifies the 'Package Properties'
void StructT0x1f::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    FutureDataLst localFutureLst{mDs};

    auto_read_prefixes(Structure::T0x1f, localFutureLst);

    readPreamble();

    name = mDs.get().readStringLenZeroTerm();

    std::string unknownStr0 = mDs.get().readStringLenZeroTerm(); // @todo figure out
    spdlog::trace("{}: unknownStr0 = {}", __func__, unknownStr0);

    refDes = mDs.get().readStringLenZeroTerm();

    std::string unknownStr1 = mDs.get().readStringLenZeroTerm(); // @todo figure out
    spdlog::trace("{}: unknownStr1 = {}", __func__, unknownStr1);

    pcbFootprint = mDs.get().readStringLenZeroTerm();

    // @todo It's probably located at the end of the structure
    // const uint16_t lenPinIdxMappings = mDs.get().readUint16();

    // spdlog::trace("lenPinIdxMappings = {}", lenPinIdxMappings);

    // for(size_t i = 0u; i < lenPinIdxMappings; ++i)
    // {
    //     pinIdxMappings.push_back(dynamic_pointer_cast<StructPinIdxMapping>(readStructure()));
    // }

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}