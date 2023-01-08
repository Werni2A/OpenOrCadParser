#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamHSObjects.hpp"


void StreamHSObjects::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: - 0", __func__));
    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: - 1", __func__));

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}