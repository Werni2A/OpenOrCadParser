#ifndef TEXTFONT_HPP
#define TEXTFONT_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class TextFont : public CommonBase
{
public:

    TextFont(DataStream& aDs) : CommonBase{aDs}, height{0}, width{0}, escapement{0},
        weight{0}, italic{0}, fontName{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    int32_t height;
    int32_t width;

    uint16_t escapement;

    uint16_t weight;

    uint16_t italic;

    std::string fontName;
};


[[maybe_unused]]
static std::string to_string(const TextFont& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}height = {}\n", indent(1), aObj.height);
    str += fmt::format("{}width  = {}\n", indent(1), aObj.width);
    str += fmt::format("{}escapement = {}\n", indent(1), aObj.escapement);
    str += fmt::format("{}weight = {}\n", indent(1), aObj.weight);
    str += fmt::format("{}italic = {}\n", indent(1), aObj.italic);
    str += fmt::format("{}fontName = {}\n", indent(1), aObj.fontName);

    return str;
}


inline std::string TextFont::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const TextFont& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // TEXTFONT_HPP