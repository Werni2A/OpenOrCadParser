#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "PinIdxMapping.hpp"


PinIdxMapping Parser::readPinIdxMapping()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    PinIdxMapping obj;

    obj.unitRef = mDs.readStringLenZeroTerm();
    obj.refDes  = mDs.readStringLenZeroTerm();

    const uint16_t pinCount = mDs.readUint16();

    // @todo Add to kaitai file i = 'Order' of pin
    // See OrCAD: 'Pin Properties' -> 'Order'
    for(size_t i = 0u; i < pinCount; ++i)
    {
        obj.pinMap.push_back(mDs.readStringLenZeroTerm());

        const uint8_t separator = mDs.readUint8();

        spdlog::debug("Sep = 0x{:02x}", separator);

        // @todo maybe this is not a separator but the additional property of the pin?
        // As soon as I add a property like NET_SHORT the separator changes from 0x7f to 0xaa
        // This is probably also affected by units and convert view.
        if(separator != 0x7f && separator != 0xaa && separator != 0xff)
        {
            throw std::runtime_error(fmt::format("Separator should be 0x{:02x}, 0x{:02x} or"
                " 0x{:02x} but got 0x{:02x}!", 0x7f, 0xaa, 0xff, separator));
        }
    }

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}