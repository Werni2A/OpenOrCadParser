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
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const size_t startOffset = mDs.get().getCurrentOffset();

    uint32_t byteLength = mDs.get().readUint32();

    // Predict version
    switch(byteLength)
    {
        case 24: aVersion = FileFormatVersion::A; break;
        case 32: aVersion = FileFormatVersion::B; break;
        case 40: aVersion = FileFormatVersion::C; break;
        default:                                  break;
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
        setFillStyle(ToFillStyle(mDs.get().readUint32()));
        setHatchStyle(ToHatchStyle(mDs.get().readInt32()));
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
    spdlog::info(to_string());
}