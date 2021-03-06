#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../General.hpp"
#include "CommentText.hpp"

#include "../Parser.hpp"


CommentText Parser::readCommentText()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    CommentText obj{&mLibrary};

    const uint32_t byteLength = mDs.readUint32();

    mDs.assumeData({0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

    obj.locX = mDs.readInt32();
    obj.locY = mDs.readInt32();

    obj.x2 = mDs.readInt32();
    obj.y2 = mDs.readInt32();
    obj.x1 = mDs.readInt32();
    obj.y1 = mDs.readInt32();

    obj.textFontIdx = mDs.readUint16();

    if(obj.textFontIdx > mLibrary.symbolsLibrary.textFonts.size())
    {
        throw std::out_of_range(std::string(__func__) + ": textFontIdx is out of range! Expected " +
            std::to_string(obj.textFontIdx) + " <= " +
            std::to_string(mLibrary.symbolsLibrary.textFonts.size()) + "!");
    }

    mDs.printUnknownData(std::clog, 2, std::string(__func__) + " - 0");

    obj.name = mDs.readStringLenZeroTerm();

    if(mDs.getCurrentOffset() != startOffset + byteLength)
    {
        throw MisinterpretedData(__func__, startOffset, byteLength, mDs.getCurrentOffset());
    }

    if(byteLength != 39u + obj.name.size())
    {
        throw FileFormatChanged("CommentText");
    }

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}


// #include <cstdint>
// #include <stdexcept>
// #include <string>

// #include "CommentText.hpp"

// #include "TextFont.hpp"
// #include "Library.hpp"


// TextFont CommentText::getFont(const Library& aLibrary) const
// {
//     const int64_t idx = static_cast<int64_t>(textFontIdx) - 1;

//     TextFont textFont;

//     if(idx >= 0)
//     {
//         // Retrieve font from the library.
//         textFont = aLibrary.symbolsLibrary.fontProperties.at(idx);
//         // @todo provide try catch block for better exception messages
//     }
//     else if(idx == -1)
//     {
//         // @todo Unknown but it is probably the default font;
//         throw std::runtime_error(std::string(__func__) + ": Check this out!");
//     }
//     else // idx < -1
//     {
//         // This should never happen.
//         throw std::runtime_error(std::string(__func__) + ": Unexpected index " + std::to_string(idx));
//     }

//     return textFont;
// }