#ifndef STRUCTGRAPHICBOXINST_HPP
#define STRUCTGRAPHICBOXINST_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "General.hpp"


struct StructGraphicBoxInst
{
    uint32_t dbId;

    int16_t locX;
    int16_t locY;

    int16_t x1;
    int16_t y1;

    int16_t x2;
    int16_t y2;

    Color color;
};


[[maybe_unused]]
static std::string to_string(const StructGraphicBoxInst& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("dbId  = {}\n", aObj.dbId);
    str += fmt::format("locX  = {}\n", aObj.locX);
    str += fmt::format("locY  = {}\n", aObj.locY);
    str += fmt::format("x1    = {}\n", aObj.x1);
    str += fmt::format("y1    = {}\n", aObj.y1);
    str += fmt::format("x2    = {}\n", aObj.x2);
    str += fmt::format("y2    = {}\n", aObj.y2);
    str += fmt::format("color = {}\n", to_string(aObj.color));
    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicBoxInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGRAPHICBOXINST_HPP