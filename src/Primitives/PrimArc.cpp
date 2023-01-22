#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Primitives/PrimArc.hpp"


size_t PrimArc::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::A)
    {
        expectedByteLength = 40u;
    }
    else
    {
        expectedByteLength = 48u;
    }

    return expectedByteLength;
}


void PrimArc::read(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const size_t startOffset = mDs.get().getCurrentOffset();

    const uint32_t byteLength = mDs.get().readUint32();

    // Predict version
    switch(byteLength)
    {
        case 40: aVersion = FileFormatVersion::A; break;
        case 48: aVersion = FileFormatVersion::B; break;
        default:                                  break;
    }

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    x1 = mDs.get().readInt32();
    y1 = mDs.get().readInt32();
    x2 = mDs.get().readInt32();
    y2 = mDs.get().readInt32();

    startX = mDs.get().readInt32();
    startY = mDs.get().readInt32();
    endX   = mDs.get().readInt32();
    endY   = mDs.get().readInt32();

    if(aVersion >= FileFormatVersion::B)
    {
        setLineStyle(ToLineStyle(mDs.get().readUint32()));
        setLineWidth(ToLineWidth(mDs.get().readUint32()));
    }

    if(mDs.get().getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.get().getCurrentOffset());
    }

    if(byteLength != getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(*this)>()));
    }

    readPreamble();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}