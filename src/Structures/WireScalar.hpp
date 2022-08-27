#ifndef WIRESCALAR_H
#define WIRESCALAR_H


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../Enums/Color.hpp"
#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"


struct WireScalar
{
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
static std::string to_string(const WireScalar& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("dbId      = {}\n", aObj.dbId);
    str += fmt::format("wireColor = {}\n", to_string(aObj.wireColor));
    str += fmt::format("startX    = {}\n", aObj.startX);
    str += fmt::format("startY    = {}\n", aObj.startY);
    str += fmt::format("endX      = {}\n", aObj.endX);
    str += fmt::format("endY      = {}\n", aObj.endY);
    str += fmt::format("wireLineWidth = {}\n", to_string(aObj.wireLineWidth));
    str += fmt::format("wireLineStyle = {}\n", to_string(aObj.wireLineStyle));

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const WireScalar& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // WIRESCALAR_H