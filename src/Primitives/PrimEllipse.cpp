#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Primitives/PrimEllipse.hpp"

size_t PrimEllipse::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    switch(aVersion)
    {
        case FileFormatVersion::A:
            expectedByteLength = 24U;
            break;

        case FileFormatVersion::B:
            expectedByteLength = 32U;
            break;

        case FileFormatVersion::C:
            expectedByteLength = 40U;
            break;

        default:
            expectedByteLength = 0U;
            break;
    }

    return expectedByteLength;
}

void PrimEllipse::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

    uint32_t byteLength = ds.readUint32();

    // Predict version
    switch(byteLength)
    {
        case 24:
            aVersion = FileFormatVersion::A;
            break;
        case 32:
            aVersion = FileFormatVersion::B;
            break;
        case 40:
            aVersion = FileFormatVersion::C;
            break;
        default:
            break;
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

    parser.readPreamble();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}