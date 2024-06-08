#ifndef STRUCTSYMBOLDISPLAYPROP_HPP
#define STRUCTSYMBOLDISPLAYPROP_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "Enums/Rotation.hpp"
#include "Record.hpp"


class StructSymbolDisplayProp : public Record
{
public:

    StructSymbolDisplayProp(StreamContext& aCtx) : Record{aCtx}, nameIdx{0}, textFontIdx{0},
        rotation{Rotation::Deg_0}, x{0}, y{0}, propColor{Color::Default}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    uint32_t nameIdx;
    uint16_t textFontIdx;

    Rotation rotation;

    int16_t x;
    int16_t y;

    Color propColor;
};

// @todo implement getter methods for name and textFont

[[maybe_unused]]
static std::string to_string(const StructSymbolDisplayProp& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}nameIdx     = {}\n", indent(1), aObj.nameIdx);
    str += fmt::format("{}textFontIdx = {}\n", indent(1), aObj.textFontIdx);
    str += fmt::format("{}rotation    = {}\n", indent(1), to_string(aObj.rotation));
    str += fmt::format("{}x = {}\n", indent(1), aObj.x);
    str += fmt::format("{}y = {}\n", indent(1), aObj.y);
    str += fmt::format("{}propColor   = {}\n", indent(1), to_string(aObj.propColor));

    return str;
}


inline std::string StructSymbolDisplayProp::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSymbolDisplayProp& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSYMBOLDISPLAYPROP_HPP