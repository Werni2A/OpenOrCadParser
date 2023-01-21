#ifndef STRUCTWIREBUS_HPP
#define STRUCTWIREBUS_HPP


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


class StructWireBus : public CommonBase
{
public:

    StructWireBus(DataStream& aDs) : CommonBase{aDs}, id{0}, color{Color::Default},
        startX{0}, startY{0}, endX{0}, endY{0},
        lineWidth{LineWidth::Default}, lineStyle{LineStyle::Default}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    uint32_t  id;
    Color     color;

    int32_t   startX;
    int32_t   startY;
    int32_t   endX;
    int32_t   endY;

    LineWidth lineWidth;
    LineStyle lineStyle;
};


[[maybe_unused]]
static std::string to_string(const StructWireBus& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}id        = {}\n", indent(1), aObj.id);
    str += fmt::format("{}color     = {}\n", indent(1), to_string(aObj.color));
    str += fmt::format("{}startX    = {}\n", indent(1), aObj.startX);
    str += fmt::format("{}startY    = {}\n", indent(1), aObj.startY);
    str += fmt::format("{}endX      = {}\n", indent(1), aObj.endX);
    str += fmt::format("{}endY      = {}\n", indent(1), aObj.endY);
    str += fmt::format("{}lineWidth = {}\n", indent(1), to_string(aObj.lineWidth));
    str += fmt::format("{}lineStyle = {}\n", indent(1), to_string(aObj.lineStyle));

    return str;
}


inline std::string StructWireBus::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructWireBus& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTWIREBUS_HPP