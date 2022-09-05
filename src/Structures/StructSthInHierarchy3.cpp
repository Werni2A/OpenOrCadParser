#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSthInHierarchy3.hpp"


StructSthInHierarchy3 Parser::readStructSthInHierarchy3()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StructSthInHierarchy3 obj;

    mDs.printUnknownData(12, fmt::format("{}: 2", __func__));

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}