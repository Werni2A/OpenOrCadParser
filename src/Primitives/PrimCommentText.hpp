#ifndef PRIMCOMMENTTEXT_HPP
#define PRIMCOMMENTTEXT_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Structures/TextFont.hpp"
#include "Primitives/PrimBase.hpp"


// Forward declaration
struct Library;


class PrimCommentText : public PrimBase
{
public:
    PrimCommentText(DataStream& aDs) : PrimBase{aDs}, locX{0}, locY{0}, name{}, x1{0}, y1{0}, x2{0}, y2{0}, textFontIdx{0}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    TextFont getTextFont() const;

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
};


[[maybe_unused]]
static std::string to_string(const PrimCommentText& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}locX = {}\n", indent(1), aObj.locX);
    str += fmt::format("{}locY = {}\n", indent(1), aObj.locY);
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}x1   = {}\n", indent(1), aObj.x1);
    str += fmt::format("{}y1   = {}\n", indent(1), aObj.y1);
    str += fmt::format("{}x2   = {}\n", indent(1), aObj.x2);
    str += fmt::format("{}y2   = {}\n", indent(1), aObj.y2);
    str += fmt::format("{}textFontIdx = {}\n", indent(1), aObj.textFontIdx);
    str += indent(to_string(aObj.getTextFont()), 2);

    return str;
}


inline std::string PrimCommentText::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PrimCommentText& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PRIMCOMMENTTEXT_HPP