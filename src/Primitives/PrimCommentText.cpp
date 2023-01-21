#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Library.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Win32/LOGFONTA.hpp"


void PrimCommentText::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const size_t startOffset = mDs.get().getCurrentOffset();

     // @todo Adding 8 Byte does not make any sense, why did it
     //       work previously and why does it work in other
     //       primitives that use similar data structures?
     //       Maybe the byte length does not count itself and
     //       the following 4 zero bytes.
     //       This issue is somehow related to the disabled
     //       readOptionalTrailingFuture check in StructPartInst
     //       and readOptionalTrailingFuture in StructT0x10
    const uint32_t byteLength = mDs.get().readUint32() + 8U;

    mDs.get().assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    locX = mDs.get().readInt32();
    locY = mDs.get().readInt32();

    spdlog::debug("locX = {}", locX);
    spdlog::debug("locY = {}", locY);

    x2 = mDs.get().readInt32();
    y2 = mDs.get().readInt32();
    x1 = mDs.get().readInt32();
    y1 = mDs.get().readInt32();

    spdlog::debug("x2 = {}", x2);
    spdlog::debug("y2 = {}", y2);
    spdlog::debug("x1 = {}", x1);
    spdlog::debug("y1 = {}", y1);

    // @todo Check if fontIdx with 4 byte fits. I.e. are the following 2 Byte all 0?
    textFontIdx = mDs.get().readUint16();

    spdlog::debug("textFontIdx = {}", textFontIdx);

    if(textFontIdx > gLibrary->library->textFonts.size())
    {
        throw std::out_of_range(std::string(__func__) + ": textFontIdx is out of range! Expected " +
            std::to_string(textFontIdx) + " <= " +
            std::to_string(gLibrary->library->textFonts.size()) + "!");
    }

    mDs.get().printUnknownData(2, std::string(__func__) + " - 1");

    name = mDs.get().readStringLenZeroTerm();

    spdlog::debug("name = {}", name);

    if(mDs.get().getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.get().getCurrentOffset());
    }

    if(byteLength != 39u + name.size())
    {
        throw FileFormatChanged("CommentText");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());

    readPreamble();
}


LOGFONTA PrimCommentText::getTextFont() const
{
    const int64_t idx = static_cast<int64_t>(textFontIdx) - 1;

    LOGFONTA textFont{};

    if(idx >= 0)
    {
        // Retrieve font from the library.
        textFont = gLibrary->library->textFonts.at(idx);
        // @todo provide try catch block for better exception messages
    }
    else if(idx == -1)
    {
        // @todo Unknown but it is probably the default font;
        throw std::runtime_error(std::string(__func__) + ": Check this out!");
    }
    else // idx < -1
    {
        // This should never happen.
        throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(idx));
    }

    return textFont;
}