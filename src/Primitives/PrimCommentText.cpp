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
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

     // @todo Adding 8 Byte does not make any sense, why did it
     //       work previously and why does it work in other
     //       primitives that use similar data structures?
     //       Maybe the byte length does not count itself and
     //       the following 4 zero bytes.
     //       This issue is somehow related to the disabled
     //       readOptionalTrailingFuture check in StructPartInst
     //       and readOptionalTrailingFuture in StructT0x10
    const uint32_t byteLength = ds.readUint32() + 8U;

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    locX = ds.readInt32();
    locY = ds.readInt32();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    x2 = ds.readInt32();
    y2 = ds.readInt32();
    x1 = ds.readInt32();
    y1 = ds.readInt32();

    spdlog::trace("x2 = {}", x2);
    spdlog::trace("y2 = {}", y2);
    spdlog::trace("x1 = {}", x1);
    spdlog::trace("y1 = {}", y1);

    // @todo Check if fontIdx with 4 byte fits. I.e. are the following 2 Byte all 0?
    textFontIdx = ds.readUint16();

    spdlog::trace("textFontIdx = {}", textFontIdx);

    if(gLibrary != nullptr)
    {
        if(gLibrary->library)
        {
            if(textFontIdx > gLibrary->library->textFonts.size())
            {
                throw std::out_of_range(fmt::format("{}: textFontIdx is out of range! Expected {} <= {}!",
                    getMethodName(this, __func__), textFontIdx, gLibrary->library->textFonts.size()));
            }
        }
    }

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 1");

    name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    if(ds.getCurrentOffset() != startOffset + byteLength)
    {
        // throw MisinterpretedData(__func__, startOffset, byteLength, ds.getCurrentOffset());
    }

    if(byteLength != 39u + name.size())
    {
        // throw FileFormatChanged("CommentText");
    }

    readPreamble();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}


LOGFONTA PrimCommentText::getTextFont() const
{
    const int64_t idx = static_cast<int64_t>(textFontIdx) - 1;

    LOGFONTA textFont{};

    if(idx >= 0)
    {
        // Retrieve font from the library.
        if(gLibrary != nullptr)
        {
            if(gLibrary->library)
            {
                textFont = gLibrary->library->textFonts.at(idx);
            }
        }
        // @todo provide try catch block for better exception messages
    }
    else if(idx == -1)
    {
        // @todo Unknown but it is probably the default font;
        // throw std::runtime_error(getMethodName(this, __func__) + ": Check this out!");
    }
    else // idx < -1
    {
        // This should never happen.
        throw std::runtime_error(getMethodName(this, __func__) + ": Unexpected index " + std::to_string(idx));
    }

    return textFont;
}