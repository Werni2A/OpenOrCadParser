#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Structures/StructGeneralProperties.hpp"


void StructGeneralProperties::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    implementationPath = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("implementationPath = {}", implementationPath);

    implementation = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("implementation = {}", implementation);

    refDes = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("refDes = {}", refDes);

    partValue = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("partValue = {}", partValue);

    const uint8_t properties = ds.readUint8();

    mCtx.mLogger.trace("properties = {}", properties);

    // Expect that upper bits are unused => 00xx xxxxb
    if((properties & 0xc0) != 0x00)
    {
        throw std::runtime_error(fmt::format("Expected 00xx xxxxb but got 0x{:02x}",
            properties & 0xc0));
    }

    const uint8_t pinProperties      =  properties       & 0x07; // Get bits 2 down to 0
    const uint8_t implementationType = (properties >> 3) & 0x07; // Get bits 5 down to 3

    pinNameVisible   =  static_cast<bool>((pinProperties & 0x01) >> 0); // Bit 0
    pinNameRotate    =  static_cast<bool>((pinProperties & 0x02) >> 1); // Bit 1
    pinNumberVisible = !static_cast<bool>((pinProperties & 0x04) >> 2); // Bit 2 - Note that this bit is inverted

    this->implementationType = ToImplementationType(implementationType);

    // @todo Maybe merge with bit field above?
    ds.assumeData({0}, getMethodName(this, __func__) + ": 0");

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}