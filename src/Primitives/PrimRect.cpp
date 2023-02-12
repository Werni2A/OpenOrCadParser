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
    size_t expectedByteLength = 24U;

    if(aVersion.optLine)
    {
        expectedByteLength += 8U;
    }

    if(aVersion.optFill)
    {
        expectedByteLength += 8U;
    }

    return expectedByteLength;
}


void PrimRect::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion.isUnknown)
    {
        aVersion = predictVersion();
    }

    const size_t startOffset = ds.getCurrentOffset();

    const uint32_t byteLength = ds.readUint32();

    // @todo better move this if-statement into Rect::checkByteLength(byteLength, version)
    if(byteLength != PrimRect::getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged("Rect");
    }

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    x1 = ds.readInt32();
    y1 = ds.readInt32();
    x2 = ds.readInt32();
    y2 = ds.readInt32();

    if(aVersion.optLine)
    {
        setLineStyle(ToLineStyle(ds.readUint32()));
        setLineWidth(ToLineWidth(ds.readUint32()));
    }

    if(aVersion.optFill)
    {
        fillStyle  = ToFillStyle(ds.readUint32());
        hatchStyle = ToHatchStyle(ds.readInt32());
    }
    else
    {
        // Set default values
        fillStyle  = FillStyle::None;
        hatchStyle = HatchStyle::LinesHorizontal;
    }

    // @todo use for all read methods.
    checkInterpretedDataLen(__func__, startOffset, ds.getCurrentOffset(), byteLength);

    readPreamble();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}