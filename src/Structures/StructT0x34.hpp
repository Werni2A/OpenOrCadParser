#ifndef STRUCTT0X34_HPP
#define STRUCTT0X34_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"


class StructT0x34 : public CommonBase
{
public:

    StructT0x34(DataStream& aDs) : CommonBase{aDs}, id{0}, color{Color::Default},
        lineStyle{LineStyle::Default}, lineWidth{LineWidth::Default}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    uint32_t  id;

    Color     color;
    LineStyle lineStyle;
    LineWidth lineWidth;
};


[[maybe_unused]]
static std::string to_string(const StructT0x34& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}id        = {}\n", indent(1), aObj.id);
    str += fmt::format("{}color     = {}\n", indent(1), to_string(aObj.color));
    str += fmt::format("{}lineStyle = {}\n", indent(1), to_string(aObj.lineStyle));
    str += fmt::format("{}lineWidth = {}\n", indent(1), to_string(aObj.lineWidth));

    return str;
}


inline std::string StructT0x34::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructT0x34& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTT0X34_HPP