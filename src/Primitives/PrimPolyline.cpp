#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Primitives/PrimPolyline.hpp"
using namespace OOCP;

size_t PrimPolyline::getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount)
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

void PrimPolyline::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        const auto predictionFunc = [this](FileFormatVersion aVersion) { this->read(aVersion); };
        aVersion                  = parser.predictVersion(predictionFunc);
    }

    const size_t startOffset = ds.getCurrentOffset();

    const uint32_t byteLength = ds.readUint32();

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    if(aVersion >= FileFormatVersion::B)
    {
        setLineStyle(ToLineStyle(ds.readUint32()));
        setLineWidth(ToLineWidth(ds.readUint32()));
    }

    const uint16_t pointCount = ds.readUint16();

    mCtx.mLogger.trace("pointCount = {}", pointCount);

    if(pointCount < 2)
    {
        throw std::runtime_error(
            "At minimum 2 'PolylinePoint's must occur but got " + std::to_string(pointCount) + " points!");
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

    if(byteLength != getExpectedStructSize(aVersion, pointCount))
    {
        // throw FileFormatChanged(std::string(nameof::nameof_type<decltype(*this)>()));
    }

    parser.readPreamble();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}