#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/FillStyle.hpp"
#include "../Enums/HatchStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "Polygon.hpp"

#include "../Parser.hpp"



size_t Polygon::getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount)
{

    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::A)
    {
        expectedByteLength = 10u + 4u * aPointCount;
    }
    else if(aVersion <= FileFormatVersion::B)
    {
        expectedByteLength = 18u + 4u * aPointCount;
    }
    else
    {
        expectedByteLength = 26u + 4u * aPointCount;
    }

    return expectedByteLength;
}


Polygon Parser::readPolygon()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Polygon obj;

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(mFileFormatVersion >= FileFormatVersion::B)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    if(mFileFormatVersion >= FileFormatVersion::C)
    {
        obj.fillStyle  = ToFillStyle(mDs.readUint32());
        obj.hatchStyle = ToHatchStyle(mDs.readInt32());
    }
    else
    {
        // Default values
        obj.fillStyle  = FillStyle::None;
        obj.hatchStyle = HatchStyle::LinesHorizontal;
    }

    const uint16_t pointCount = mDs.readUint16();

    if(pointCount < 3u)
    {
        throw std::runtime_error("The XSD allows Polygons with < 3 points but does this make any sense? Got "
                                 + std::to_string(pointCount) + "!");
    }

    for(size_t i = 0u; i < pointCount; ++i)
    {
        obj.points.push_back(readPoint());
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != obj.getExpectedStructSize(mFileFormatVersion, pointCount))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(obj)>()));
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}