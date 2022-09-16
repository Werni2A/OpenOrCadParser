#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
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


PrimEllipse Parser::readPrimEllipse(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    PrimEllipse obj;

    uint32_t byteLength = mDs.readUint32();

    // Predict version
    switch(byteLength)
    {
        case 24: aVersion = FileFormatVersion::A; break;
        case 32: aVersion = FileFormatVersion::B; break;
        case 40: aVersion = FileFormatVersion::C; break;
        default:                                  break;
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
        obj.setFillStyle(ToFillStyle(mDs.readUint32()));
        obj.setHatchStyle(ToHatchStyle(mDs.readInt32()));
    }

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != obj.getExpectedStructSize(aVersion))
    {
        throw FileFormatChanged(std::string(nameof::nameof_type<decltype(obj)>()));
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}