#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../General.hpp"
#include "Point.hpp"

#include "../Parser.hpp"


Point Parser::readPoint()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    Point obj;

    obj.y = mDs.readUint16();
    obj.x = mDs.readUint16();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}