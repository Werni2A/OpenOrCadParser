#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Primitives/PrimCommentText.hpp"
#include "Structures/TextFont.hpp"


PrimCommentText Parser::readPrimCommentText(FileFormatVersion aVersion)
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const size_t startOffset = mDs.getCurrentOffset();

    PrimCommentText obj{&mLibrary};

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.locX = mDs.readInt32();
    obj.locY = mDs.readInt32();

    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();
    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();

    obj.textFontIdx = mDs.readUint16();

    if(obj.textFontIdx > mLibrary.library.textFonts.size())
    {
        throw std::out_of_range(std::string(__func__) + ": textFontIdx is out of range! Expected " +
            std::to_string(obj.textFontIdx) + " <= " +
            std::to_string(mLibrary.library.textFonts.size()) + "!");
    }

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    obj.name = mDs.readStringLenZeroTerm();

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != 39u + obj.name.size())
    {
        throw FileFormatChanged("CommentText");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}


TextFont PrimCommentText::getTextFont() const
{
    if(mLibrary == nullptr)
    {
        throw std::logic_error(std::string(__func__) + ": mLibrary should be set!");
    }

    const int64_t idx = static_cast<int64_t>(textFontIdx) - 1;

    TextFont textFont;

    if(idx >= 0)
    {
        // Retrieve font from the library.
        textFont = mLibrary->library.textFonts.at(idx);
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