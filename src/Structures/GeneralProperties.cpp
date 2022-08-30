#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "GeneralProperties.hpp"


GeneralProperties Parser::readGeneralProperties()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    GeneralProperties obj;

    // @todo move to kaitai file
    // doc: |
    //   Implementation path of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation Path'
    obj.implementationPath = mDs.readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Implementation of the symbol.
    //   See OrCAD: 'Part Properties' -> 'Implementation'
    obj.implementation = mDs.readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Reference descriptor for the symbol. E.g. 'R' for resistor.
    //   See OrCAD: 'Package Properties' -> 'Part Reference Prefix'
    obj.refDes = mDs.readStringLenZeroTerm();

    // @todo move to kaitai file
    // doc: |
    //   Value of the symbol. E.g. '10k' for a resistor.
    //   See OrCAD: 'Part Properties' -> 'Value'
    obj.partValue = mDs.readStringLenZeroTerm();

    const uint8_t properties = mDs.readUint8();

    // Expect that upper bits are unused => 00xx xxxxb
    if((properties & 0xc0) != 0x00)
    {
        throw std::runtime_error(fmt::format("Expected 00xx xxxxb but got 0x{:02x}",
            properties & 0xc0));
    }

    const uint8_t pinProperties      =  properties       & 0x07; // Get bits 2 down to 0
    const uint8_t implementationType = (properties >> 3) & 0x07; // Get bits 5 down to 3

    obj.pinNameVisible   =  static_cast<bool>((pinProperties & 0x01) >> 0); // Bit 0
    obj.pinNameRotate    =  static_cast<bool>((pinProperties & 0x02) >> 1); // Bit 1
    obj.pinNumberVisible = !static_cast<bool>((pinProperties & 0x04) >> 2); // Bit 2 - Note that this bit is inverted

    obj.implementationType = ToImplementationType(implementationType);

    mDs.printUnknownData(1, std::string(__func__) + " - 0");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}