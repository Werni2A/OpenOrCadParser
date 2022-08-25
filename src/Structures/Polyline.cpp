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


static FileFormatVersion predictVersion(DataStream& aDs, Parser& aParser)
{
    FileFormatVersion prediction = FileFormatVersion::Unknown;


    const std::vector<FileFormatVersion> versions{
        FileFormatVersion::A,
        FileFormatVersion::B,
        FileFormatVersion::C
    };

    const size_t initial_offset = aDs.getCurrentOffset();

    for(const auto& version : versions)
    {
        bool found = true;

        try
        {
            aParser.readPolyline(version);
        }
        catch(...)
        {
            found = false;
        }

        aDs.setCurrentOffset(initial_offset);

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


Polyline Parser::readPolyline(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    // Predict version
    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion(mDs, *this);
        // spdlog::info("Predicted version {} in {}", aVersion, __func__);
    }

    const size_t startOffset = mDs.getCurrentOffset();

    Polyline obj;

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    if(aVersion >= FileFormatVersion::A)
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

    if(byteLength != obj.getExpectedStructSize(aVersion, pointCount))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(obj)>()));
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}