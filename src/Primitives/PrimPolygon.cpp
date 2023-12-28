#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/FillStyle.hpp"
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Primitives/PrimPolygon.hpp"


size_t PrimPolygon::getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount)
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


void PrimPolygon::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        const auto predictionFunc = [this](FileFormatVersion aVersion){ this->read(aVersion); };
        aVersion = parser.predictVersion(predictionFunc);
    }

    const size_t startOffset = ds.getCurrentOffset();

    const uint32_t byteLength = ds.readUint32();

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    if(mCtx.mFileFormatVersion >= FileFormatVersion::B)
    {
        setLineStyle(ToLineStyle(ds.readUint32()));
        setLineWidth(ToLineWidth(ds.readUint32()));
    }

    if(mCtx.mFileFormatVersion >= FileFormatVersion::C)
    {
        fillStyle  = ToFillStyle(ds.readUint32());
        hatchStyle = ToHatchStyle(ds.readInt32());
    }
    else
    {
        // Default values
        fillStyle  = FillStyle::None;
        hatchStyle = HatchStyle::LinesHorizontal;
    }

    const uint16_t pointCount = ds.readUint16();

    mCtx.mLogger.trace("pointCount = {}", pointCount);

    if(pointCount < 3u)
    {
        // I've seen 2 points, even placed at the same coordinate
        // throw std::runtime_error("The XSD allows Polygons with < 3 points but does this make any sense? Got "
        //                          + std::to_string(pointCount) + "!");
    }

    for(size_t i = 0u; i < pointCount; ++i)
    {
        Point point{mCtx};
        point.read();
        points.push_back(point);
    }

    // @todo
    int byteDiff = static_cast<int>(byteLength) - static_cast<int>(ds.getCurrentOffset() - startOffset);
    if(byteDiff > 0)
    {
        ds.printUnknownData(byteDiff);
    }

    if(ds.getCurrentOffset() != startOffset + byteLength)
    {
        // throw MisinterpretedData(__func__, startOffset, byteLength, ds.getCurrentOffset());
    }

    if(byteLength != getExpectedStructSize(mCtx.mFileFormatVersion, pointCount))
    {
        // throw FileFormatChanged(std::string(nameof::nameof_type<decltype(*this)>()));
    }

    parser.readPreamble();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}