#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "Ellipse.hpp"

#include "../Parser.hpp"


size_t Ellipse::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::B)
    {
        expectedByteLength = 32u;
    }
    else
    {
        expectedByteLength = 40u;
    }

    return expectedByteLength;
}


Ellipse Parser::readEllipse(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    Ellipse obj;

    uint32_t byteLength = mDs.readUint32();

    // Predict version
    switch(byteLength)
    {
        case 32: aVersion = FileFormatVersion::A; break;
        case 40: aVersion = FileFormatVersion::C; break;
        default:                                  break;
    }

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();
    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();

    if(aVersion >= FileFormatVersion::A)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    if(aVersion >= FileFormatVersion::C)
    {
        obj.setFillStyle(ToFillStyle(mDs.readUint32()));
        obj.setHatchStyle(ToHatchStyle(mDs.readInt32()));
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