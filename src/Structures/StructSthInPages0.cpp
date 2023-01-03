#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/Primitive.hpp"
#include "FutureData.hpp"
#include "General.hpp"

#include "Parser.hpp"
#include "Structures/StructSthInPages0.hpp"


// @todo Probably a wrapper for Inst (Instances)
StructSthInPages0 Parser::readStructSthInPages0()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructSthInPages0 obj;

    mDs.printUnknownData(6, std::string(__func__) + " - 0");
    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    const uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();

        readPrimitive(primitive);
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}