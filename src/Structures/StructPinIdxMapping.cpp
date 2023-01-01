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

    readPreamble();

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

        // Bit 7         : Pin Ignore
        // Bit 6 downto 0: Pin Group
        const uint8_t bitMapPinGrpCfg = mDs.readUint8();

        // 0 = No
        // 1 = Yes
        obj.pinIgnore.push_back(GetBit(7, bitMapPinGrpCfg));

        // @note The special case of value 127 that represents an empty group
        obj.pinGroup.push_back(bitMapPinGrpCfg & 0x7f);

        spdlog::debug("pinIgnore = {}", obj.pinIgnore[i]);
        const std::string strPinGroup = (obj.pinGroup[i] != 127U) ? std::to_string(obj.pinGroup[i]) : "";
        spdlog::debug("pinGroup  = {:>3}", strPinGroup);
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}