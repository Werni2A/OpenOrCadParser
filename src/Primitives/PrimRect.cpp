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


FileFormatVersion PrimRect::predictVersion()
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


void PrimRect::read(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.get().getCurrentOffset()));

    // Predict version
    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
        // spdlog::info("Predicted version {} in {}", aVersion, __func__);
    }

    const size_t startOffset = mDs.get().getCurrentOffset();

    const uint32_t byteLength = mDs.get().readUint32();

    // @todo better move this if-statement into Rect::checkByteLength(byteLength, version)
    if(byteLength != PrimRect::getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged("Rect");
    }

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    x1 = mDs.get().readInt32();
    y1 = mDs.get().readInt32();
    x2 = mDs.get().readInt32();
    y2 = mDs.get().readInt32();

    if(aVersion >= FileFormatVersion::B)
    {
        setLineStyle(ToLineStyle(mDs.get().readUint32()));
        setLineWidth(ToLineWidth(mDs.get().readUint32()));
    }

    if(aVersion >= FileFormatVersion::C)
    {
        fillStyle  = ToFillStyle(mDs.get().readUint32());
        hatchStyle = ToHatchStyle(mDs.get().readInt32());
    }
    else
    {
        // Set default values
        fillStyle  = FillStyle::None;
        hatchStyle = HatchStyle::LinesHorizontal;
    }

    // @todo use for all read methods.
    checkInterpretedDataLen(__func__, startOffset, mDs.get().getCurrentOffset(), byteLength);

    readPreamble();

    spdlog::debug(getClosingMsg(__func__, mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}