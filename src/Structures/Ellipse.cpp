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


Ellipse Parser::readEllipse()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Ellipse obj;

    uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();
    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();

    if(mFileFormatVersion >= FileFormatVersion::A)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    if(mFileFormatVersion >= FileFormatVersion::C)
    {
        obj.setFillStyle(ToFillStyle(mDs.readUint32()));
        obj.setHatchStyle(ToHatchStyle(mDs.readInt32()));
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != obj.getExpectedStructSize(mFileFormatVersion))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(obj)>()));
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}