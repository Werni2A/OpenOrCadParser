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
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    // @todo move to kaitai file
    // doc: |
    //   Implementation path of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation Path'
    implementationPath = mDs.get().readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Implementation of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation'
    implementation = mDs.get().readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Reference descriptor for the symbol. E.g. 'R' for resistor.
    //   See OrCAD: 'Package Properties' -> 'Part Reference Prefix'
    refDes = mDs.get().readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Value of the symbol. E.g. '10k' for a resistor.
    //   See OrCAD: 'Part Properties' -> 'Value'
    partValue = mDs.get().readStringLenZeroTerm();

    const uint8_t properties = mDs.get().readUint8();

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

    mDs.get().printUnknownData(1, getMethodName(this, __func__) + ": 0");

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}