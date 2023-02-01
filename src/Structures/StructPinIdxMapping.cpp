#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructPinIdxMapping.hpp"


void StructPinIdxMapping::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::PinIdxMapping, localFutureLst);

    readPreamble();

    unitRef = ds.readStringLenZeroTerm();
    refDes  = ds.readStringLenZeroTerm();

    const uint16_t pinCount = ds.readUint16();

    spdlog::trace("pinCount = {}", pinCount);

    // @todo Add to kaitai file i = 'Order' of pin
    // See OrCAD: 'Pin Properties' -> 'Order'
    for(size_t i = 0u; i < pinCount; ++i)
    {
        const auto currOffset = ds.getCurrentOffset();
        const int16_t strLen = ds.readInt16();
        ds.setCurrentOffset(currOffset);

        // @todo What means a string length of -1?
        //       Maybe I should return from the read string
        //       function directly and return an empty optional?
        //       Probably the meaning is that the string is not valid at all
        //       strLen =  1 ->   0x01 0x00 0x41 0x00 = 'A'
        //       strLen =  0 ->   0x00 0x00 0x00      = ''
        //       strLen = -1 ->   0xff 0xff           = ?
        if(strLen == -1)
        {
            const int16_t strLen = ds.readInt16();

            spdlog::trace("strLen = {}", strLen);

            continue;
        }

        pinMap.push_back(ds.readStringLenZeroTerm());

        // Bit 7         : Pin Ignore
        // Bit 6 downto 0: Pin Group
        const uint8_t bitMapPinGrpCfg = ds.readUint8();

        // 0 = No
        // 1 = Yes
        pinIgnore.push_back(GetBit(7, bitMapPinGrpCfg));

        // @note The special case of value 127 that represents an empty group
        pinGroup.push_back(bitMapPinGrpCfg & 0x7f);

        spdlog::trace("pinIgnore = {}", pinIgnore[i]);
        const std::string strPinGroup = (pinGroup[i] != 127U) ? std::to_string(pinGroup[i]) : "";
        spdlog::trace("pinGroup  = {:>3}", strPinGroup);
    }

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}