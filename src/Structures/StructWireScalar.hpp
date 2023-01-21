#ifndef STRUCTWIRESCALAR_HPP
#define STRUCTWIRESCALAR_HPP


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


class StructWireScalar : public CommonBase
{
public:

    StructWireScalar(DataStream& aDs) : CommonBase{aDs}, dbId{0}, wireColor{Color::Default},
        startX{0}, startY{0}, endX{0}, endY{0}, wireLineWidth{LineWidth::Default},
        wireLineStyle{LineStyle::Default}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    uint32_t dbId;

    Color wireColor;

    int32_t startX;
    int32_t startY;
    int32_t endX;
    int32_t endY;

    LineWidth wireLineWidth;
    LineStyle wireLineStyle;
};


[[maybe_unused]]
static std::string to_string(const StructWireScalar& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}dbId      = {}\n", indent(1), aObj.dbId);
    str += fmt::format("{}wireColor = {}\n", indent(1), to_string(aObj.wireColor));
    str += fmt::format("{}startX    = {}\n", indent(1), aObj.startX);
    str += fmt::format("{}startY    = {}\n", indent(1), aObj.startY);
    str += fmt::format("{}endX      = {}\n", indent(1), aObj.endX);
    str += fmt::format("{}endY      = {}\n", indent(1), aObj.endY);
    str += fmt::format("{}wireLineWidth = {}\n", indent(1), to_string(aObj.wireLineWidth));
    str += fmt::format("{}wireLineStyle = {}\n", indent(1), to_string(aObj.wireLineStyle));

    return str;
}


inline std::string StructWireScalar::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructWireScalar& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTWIRESCALAR_HPP