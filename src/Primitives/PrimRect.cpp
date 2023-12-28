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
#include "GenericParser.hpp"
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


void PrimRect::read(FileFormatVersion aVersion)
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

    if(aVersion >= FileFormatVersion::B)
    {
        setLineStyle(ToLineStyle(ds.readUint32()));
        setLineWidth(ToLineWidth(ds.readUint32()));
    }

    if(aVersion >= FileFormatVersion::C)
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
    parser.checkInterpretedDataLen(__func__, startOffset, ds.getCurrentOffset(), byteLength);

    parser.readPreamble();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}