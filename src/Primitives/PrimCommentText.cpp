#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

#include <nameof.hpp>

#include "Database.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "GetStreamHelper.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Win32/LOGFONTA.hpp"


void PrimCommentText::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    const auto lib = getLibraryStreamFromDb(mCtx.mDb);

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const size_t startOffset = ds.getCurrentOffset();

     // @todo Adding 8 Byte does not make any sense, why did it
     //       work previously and why does it work in other
     //       primitives that use similar data structures?
     //       Maybe the byte length does not count itself and
     //       the following 4 zero bytes.
     //       This issue is somehow related to the disabled
     //       readOptionalTrailingFuture check in StructPlacedInstance
     //       and readOptionalTrailingFuture in StructT0x10
    const uint32_t byteLength = ds.readUint32() + 8U;

    ds.assumeData({0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

    locX = ds.readInt32();
    locY = ds.readInt32();

    mCtx.mLogger.trace("locX = {}", locX);
    mCtx.mLogger.trace("locY = {}", locY);

    x2 = ds.readInt32();
    y2 = ds.readInt32();
    x1 = ds.readInt32();
    y1 = ds.readInt32();

    mCtx.mLogger.trace("x2 = {}", x2);
    mCtx.mLogger.trace("y2 = {}", y2);
    mCtx.mLogger.trace("x1 = {}", x1);
    mCtx.mLogger.trace("y1 = {}", y1);

    // @todo Check if fontIdx with 4 byte fits. I.e. are the following 2 Byte all 0?
    textFontIdx = ds.readUint16();

    mCtx.mLogger.trace("textFontIdx = {}", textFontIdx);

    if(lib)
    {
        if(textFontIdx > lib->textFonts.size())
        {
            throw std::out_of_range(fmt::format("{}: textFontIdx is out of range! Expected {} <= {}!",
                getMethodName(this, __func__), textFontIdx, lib->textFonts.size()));
        }
    }

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 1");

    name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("name = {}", name);

    if(ds.getCurrentOffset() != startOffset + byteLength)
    {
        // throw MisinterpretedData(__func__, startOffset, byteLength, ds.getCurrentOffset());
    }

    if(byteLength != 39u + name.size())
    {
        // throw FileFormatChanged("CommentText");
    }

    parser.readPreamble();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}


LOGFONTA PrimCommentText::getTextFont() const
{
    const auto lib = getLibraryStreamFromDb(mCtx.mDb);

    const int64_t idx = static_cast<int64_t>(textFontIdx) - 1;

    LOGFONTA textFont{};

    if(idx >= 0)
    {
        // Retrieve font from `Library`
        if(lib)
        {
            if(static_cast<std::size_t>(idx) < lib->textFonts.size())
            {
                textFont = lib->textFonts.at(idx);
            }
            else
            {
                mCtx.mLogger.warn("Index is out-of-range: {} vs {}", idx, lib->textFonts.size());
            }
        }
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