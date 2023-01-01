#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/FillStyle.hpp"
#include "Enums/HatchStyle.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Primitives/PrimRect.hpp"


size_t PrimRect::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::A)
    {
        expectedByteLength = 24U;
    }
    else if(aVersion <= FileFormatVersion::B)
    {
        expectedByteLength = 32u;
    }
    else
    {
        expectedByteLength = 40u;
    }

    return expectedByteLength;
}


[[maybe_unused]]
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
            aParser.readPrimRect(version);
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


PrimRect Parser::readPrimRect(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    // Predict version
    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion(mDs, *this);
        // spdlog::info("Predicted version {} in {}", aVersion, __func__);
    }

    const size_t startOffset = mDs.getCurrentOffset();

    PrimRect obj;

    const uint32_t byteLength = mDs.readUint32();

    // @todo better move this if-statement into Rect::checkByteLength(byteLength, version)
    if(byteLength != PrimRect::getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged("Rect");
    }

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();
    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();

    if(aVersion >= FileFormatVersion::B)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    if(aVersion >= FileFormatVersion::C)
    {
        obj.fillStyle  = ToFillStyle(mDs.readUint32());
        obj.hatchStyle = ToHatchStyle(mDs.readInt32());
    }
    else
    {
        // Set default values
        obj.fillStyle  = FillStyle::None;
        obj.hatchStyle = HatchStyle::LinesHorizontal;
    }

    // @todo use for all read methods.
    checkInterpretedDataLen(__func__, startOffset, mDs.getCurrentOffset(), byteLength);

    readPreamble();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}