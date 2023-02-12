#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Primitives/PrimEllipse.hpp"


size_t PrimEllipse::getExpectedStructSize(FileFormatVersion aVersion)
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


void PrimEllipse::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion.isUnknown)
    {
        aVersion = predictVersion();
    }

    const size_t startOffset = ds.getCurrentOffset();

    uint32_t byteLength = ds.readUint32();

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
        setFillStyle(ToFillStyle(ds.readUint32()));
        setHatchStyle(ToHatchStyle(ds.readInt32()));
    }

    if(ds.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, ds.getCurrentOffset());
    }

    if(byteLength != getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(*this)>()));
    }

    readPreamble();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}