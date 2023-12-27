#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamAdminData.hpp"


void StreamAdminData::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    uint8_t sth0 = ds.readUint8();

    // @todo only those two values have been observed until now
    if(sth0 != 0 && sth0 != 1)
    {
        spdlog::warn("{}: Found new value. Check it out!", getMethodName(this, __func__));
    }

    ds.assumeData({0x00, 0x00, 0x00, 0x00, 0x00},
        fmt::format("{}: Found actually useful data!", getMethodName(this, __func__)));

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}