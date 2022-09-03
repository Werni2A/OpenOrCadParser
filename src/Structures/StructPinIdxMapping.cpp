#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructPinIdxMapping.hpp"


StructPinIdxMapping Parser::readStructPinIdxMapping()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructPinIdxMapping obj;

    obj.unitRef = mDs.readStringLenZeroTerm();
    obj.refDes  = mDs.readStringLenZeroTerm();

    const uint16_t pinCount = mDs.readUint16();

    spdlog::debug("pinCount = {}", pinCount);

    // @todo Add to kaitai file i = 'Order' of pin
    // See OrCAD: 'Pin Properties' -> 'Order'
    for(size_t i = 0u; i < pinCount; ++i)
    {
        const auto currOffset = mDs.getCurrentOffset();
        const int16_t strLen = mDs.readInt16();
        mDs.setCurrentOffset(currOffset);

        // @todo What means a string length of -1?
        //       Maybe I should return from the read string
        //       function directly and return an empty optional?
        //       Probably the meaning is that the string is not valid at all
        //       strLen =  1 ->   0x01 0x00 0x41 0x00 = 'A'
        //       strLen =  0 ->   0x00 0x00 0x00      = ''
        //       strLen = -1 ->   0xff 0xff           = ?
        if(strLen == -1)
        {
            const int16_t strLen = mDs.readInt16();
            continue;
        }

        obj.pinMap.push_back(mDs.readStringLenZeroTerm());

        const uint8_t separator = mDs.readUint8();

        spdlog::debug("Sep = 0x{:02x}", separator);

        // @todo maybe this is not a separator but the additional property of the pin?
        // As soon as I add a property like NET_SHORT the separator changes from 0x7f to 0xaa
        // This is probably also affected by units and convert view.
        // @todo Probably 0xff belongs to the strLen from above, then only 0x7f and 0xaa are valid values
        //        Verify this by removing it from the if-statement
        if(separator != 0x7f && separator != 0xaa && separator != 0xff)
        {
            throw std::runtime_error(fmt::format("Separator should be 0x{:02x}, 0x{:02x} or"
                " 0x{:02x} but got 0x{:02x}!", 0x7f, 0xaa, 0xff, separator));
        }
    }

    sanitizeThisFutureSize(thisFuture);

    // @todo discard trailing data if present. This should fix a few errors
    checkTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}