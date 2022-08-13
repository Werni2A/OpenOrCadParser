#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/FillStyle.hpp"
#include "../Enums/HatchStyle.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "Rect.hpp"

#include "../Parser.hpp"


size_t Rect::getExpectedStructSize(FileFormatVersion aVersion)
{
    size_t expectedByteLength;

    if(aVersion <= FileFormatVersion::B)
    {
        expectedByteLength = 32u;
    }
    else
    {
        expectedByteLength = 40u;
    }

    return expectedByteLength;
}


Rect Parser::readRect()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    Rect obj;

    const uint32_t byteLength = mDs.readUint32();

    // @todo better move this if-statement into Rect::checkByteLength(byteLength, version)
    if(byteLength != Rect::getExpectedStructSize(mFileFormatVersion))
    {
        throw FileFormatChanged("Rect");
    }

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();
    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();

    if(mFileFormatVersion >= FileFormatVersion::A)
    {
        obj.setLineStyle(ToLineStyle(mDs.readUint32()));
        obj.setLineWidth(ToLineWidth(mDs.readUint32()));
    }

    if(mFileFormatVersion >= FileFormatVersion::C)
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

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}