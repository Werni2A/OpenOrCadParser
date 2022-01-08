#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "Polyline.hpp"

#include "../Parser.hpp"


size_t Polyline::getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::A)
    {
        expectedByteLength = 10u + 4u * aPointCount;
    }
    else
    {
        expectedByteLength = 18u + 4u * aPointCount;
    }

    return expectedByteLength;
}


Polyline Parser::readPolyline()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Polyline obj;

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(mFileFormatVersion >= FileFormatVersion::A)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    const uint16_t pointCount = mDs.readUint16();

    if(pointCount < 2)
    {
        throw std::runtime_error("At minimum 2 'PolylinePoint's must occur but got " + std::to_string(pointCount)
                                 + " points!");
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