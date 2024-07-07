#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Primitives/PrimLine.hpp"

size_t OOCP::PrimLine::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::A)
    {
        expectedByteLength = 24u;
    }
    else
    {
        expectedByteLength = 32u;
    }

    return expectedByteLength;
}

void OOCP::PrimLine::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

    const uint32_t byteLength = ds.readUint32();

    // Predict version
    switch(byteLength)
    {
        case 24:
            aVersion = FileFormatVersion::A;
            break;
        case 32:
            aVersion = FileFormatVersion::B;
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