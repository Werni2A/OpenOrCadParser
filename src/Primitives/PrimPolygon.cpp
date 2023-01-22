#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/FillStyle.hpp"
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
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


FileFormatVersion PrimPolygon::predictVersion()
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;


    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

    const size_t initial_offset = mDs.get().getCurrentOffset();

    for(const auto& version : versions)
    {
        bool found = true;

        try
        {
            read(version);
        }
        catch(...)
        {
            found = false;
        }

        mDs.get().setCurrentOffset(initial_offset);

        if(found)
        {
            prediction = version;
            break;
        }
    }

    if(prediction == FileFormatVersion::Unknown)
    {
        // Set to previous default value
        // s.t. tests not fail
        prediction = FileFormatVersion::C;
    }

    return prediction;
}


void PrimPolygon::read(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // Predict version
    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
        // spdlog::debug("Predicted version {} in {}", static_cast<int>(aVersion), __func__);
    }

    const size_t startOffset = mDs.get().getCurrentOffset();

    const uint32_t byteLength = mDs.get().readUint32();

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(gFileFormatVersion >= FileFormatVersion::B)
    {
        setLineStyle(ToLineStyle(mDs.get().readUint32()));
        setLineWidth(ToLineWidth(mDs.get().readUint32()));
    }

    if(gFileFormatVersion >= FileFormatVersion::C)
    {
        fillStyle  = ToFillStyle(mDs.get().readUint32());
        hatchStyle = ToHatchStyle(mDs.get().readInt32());
    }
    else
    {
        // Default values
        fillStyle  = FillStyle::None;
        hatchStyle = HatchStyle::LinesHorizontal;
    }

    const uint16_t pointCount = mDs.get().readUint16();

    if(pointCount < 3u)
    {
        throw std::runtime_error("The XSD allows Polygons with < 3 points but does this make any sense? Got "
                                 + std::to_string(pointCount) + "!");
    }

    for(size_t i = 0u; i < pointCount; ++i)
    {
        Point point{mDs};
        point.read();
        points.push_back(point);
    }

    if(mDs.get().getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.get().getCurrentOffset());
    }

    if(byteLength != getExpectedStructSize(gFileFormatVersion, pointCount))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(*this)>()));
    }

    readPreamble();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}