#include <cmath>
#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "Bezier.hpp"

#include "../Parser.hpp"


size_t Bezier::getExpectedStructSize(FileFormatVersion aVersion, size_t aPointCount)
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


Bezier Parser::readBezier()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    Bezier obj;

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(mFileFormatVersion >= FileFormatVersion::B)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    const uint16_t pointCount = mDs.readUint16();

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

        const auto isInteger = [](float k) -> bool
        {
            // @todo FP comparisons are a bad idea. Check for e.g. 10 maschine epsilon difference
            return std::floor(k) == k;
        };

        if(!isInteger((pointCount - 1.0) / 3.0))
        {
            throw std::runtime_error("Number of 'BezierPoint's is incorrect. Got " + std::to_string(pointCount)
                                    + " points!");
        }
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