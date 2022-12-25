#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"
#include "Streams/StreamAdminData.hpp"


StreamAdminData Parser::readStreamAdminData()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StreamAdminData obj;

    uint8_t sth0 = mDs.readUint8();

    // @todo only those two values have been observed until now
    if(sth0 != 0 && sth0 != 1)
    {
        spdlog::critical("{}: Found new value. Check it out!", __func__);
    }

    mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: Found actually useful data!", __func__));

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}