#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSthInHierarchy1.hpp"


StructSthInHierarchy1 Parser::readStructSthInHierarchy1()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StructSthInHierarchy1 obj;

    mDs.printUnknownData(27, fmt::format("{}: 0", __func__));

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}