#ifndef STRUCTT0X35_HPP
#define STRUCTT0X35_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Record.hpp"


class StructT0x35 : public Record
{
public:

    StructT0x35(StreamContext& aCtx) : Record{aCtx}, id{0}, color{Color::Default},
        lineStyle{LineStyle::Default}, lineWidth{LineWidth::Default}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Structure getObjectType() const override
    {
        return Structure::T0x35;
    }

    uint32_t  id;
    Color     color;
    LineStyle lineStyle;
    LineWidth lineWidth;
};


[[maybe_unused]]
static std::string to_string(const StructT0x35& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}id        = {}\n", indent(1), aObj.id);
    str += fmt::format("{}color     = {}\n", indent(1), to_string(aObj.color));
    str += fmt::format("{}lineStyle = {}\n", indent(1), to_string(aObj.lineStyle));
    str += fmt::format("{}lineWidth = {}\n", indent(1), to_string(aObj.lineWidth));

    return str;
}


inline std::string StructT0x35::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructT0x35& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTT0X35_HPP