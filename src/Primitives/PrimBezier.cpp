#include <cmath>
#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Primitives/PrimBezier.hpp"


size_t PrimBezier::getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount)
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


void PrimBezier::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        const auto predictionFunc = [this](FileFormatVersion aVersion){ this->read(aVersion); };
        aVersion = parser.predictVersion(predictionFunc);
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

    spdlog::trace("pointCount = {}", pointCount);

    if(pointCount < 4)
    {
        throw std::runtime_error("At minimum 4 'BezierPoint's must occur but got " + std::to_string(pointCount)
                                 + " points!");
    }
    else
    {
        // Check correct number of points.

        // Example showing the overlapping ends of the points
        // Bezier Segment 0: P0P1P2P3
        // Bezier Segment 1:       P0P1P2P3
        // Bezier Segment 2:             P0P1P2P3
        // Bezier Segment 3:                   P0P1P2P3

        // n_b = Number of Bezier segments
        // n_p = Number of BezierPoints

        //     3 * n_b + 1 = n_p
        // <=> n_b = (n_p - 1) / 3

        // Quotient = Dividend / Divisor
        const auto isDivisible = [](int aDividend, int aDivisor) -> bool
        {
            aDividend = std::abs(aDividend);
            aDivisor  = std::abs(aDivisor);

            const auto rest = aDividend % aDivisor;

            return rest == 0;
        };

        if(!isDivisible(pointCount - 1, 3))
        {
            throw std::runtime_error("Number of 'BezierPoint's is incorrect. Got " + std::to_string(pointCount)
                                    + " points!");
        }
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

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}