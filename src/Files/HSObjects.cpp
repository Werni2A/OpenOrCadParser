#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "HSObjects.hpp"

#include "../Parser.hpp"


HSObjects Parser::readHSObjects()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    HSObjects obj;

    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: - 0", __func__));
    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: - 1", __func__));

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}