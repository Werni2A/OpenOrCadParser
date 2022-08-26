#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "Arc.hpp"

#include "../Parser.hpp"


size_t Arc::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::A)
    {
        expectedByteLength = 40u;
    }
    else
    {
        expectedByteLength = 48u;
    }

    return expectedByteLength;
}


Arc Parser::readArc(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    Arc obj;

    const uint32_t byteLength = mDs.readUint32();

    // Predict version
    switch(byteLength)
    {
        case 40: aVersion = FileFormatVersion::A; break;
        case 48: aVersion = FileFormatVersion::B; break;
        default:                                  break;
    }

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();
    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();

    obj.startX = mDs.readInt32();
    obj.startY = mDs.readInt32();
    obj.endX   = mDs.readInt32();
    obj.endY   = mDs.readInt32();

    if(aVersion >= FileFormatVersion::B)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != obj.getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(obj)>()));
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}