

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