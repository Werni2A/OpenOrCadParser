#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamHSObjects.hpp"


void StreamHSObjects::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ds.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: - 0", __func__));
    ds.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: - 1", __func__));

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}