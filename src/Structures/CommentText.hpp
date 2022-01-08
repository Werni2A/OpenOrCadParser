#ifndef COMMENTTEXT_H
#define COMMENTTEXT_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "TextFont.hpp"


// Forward declaration
struct Library;


struct CommentText
{
    int32_t locX;
    int32_t locY;

    std::string name; /// @note This is the content of the text field
                      /// @note The XML file stores the text with HTML
                      ///       escaped characters but this is not the
                      ///       case for the binary format.

    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;

    uint16_t textFontIdx;

    const Library* mLibrary;

    CommentText(const Library* aLibrary) : mLibrary(aLibrary) {}
    // @todo Passing a nullptr should be possible but then we need
    //       std::optional<CommentText> as return type
    // CommentText() : CommentText{nullptr} {}

    TextFont getTextFont() const;
};


[[maybe_unused]]
static std::string to_string(const CommentText& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "locX = " + std::to_string(aObj.locX) + newLine();
    str += indent(1) + "locY = " + std::to_string(aObj.locY) + newLine();
    str += indent(1) + "name = " + aObj.name + newLine();
    str += indent(1) + "x1   = " + std::to_string(aObj.x1) + newLine();
    str += indent(1) + "y1   = " + std::to_string(aObj.y1) + newLine();
    str += indent(1) + "x2   = " + std::to_string(aObj.x2) + newLine();
    str += indent(1) + "y2   = " + std::to_string(aObj.y2) + newLine();
    str += indent(1) + "textFontIdx = " + std::to_string(aObj.textFontIdx) + newLine();
    str += indent(to_string(aObj.getTextFont()), 2);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const CommentText& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // COMMENTTEXT_H