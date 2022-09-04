#ifndef STRUCTGRAPHICCOMMENTTEXTINST_HPP
#define STRUCTGRAPHICCOMMENTTEXTINST_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"


struct StructGraphicCommentTextInst
{

};


[[maybe_unused]]
static std::string to_string(const StructGraphicCommentTextInst& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    // str += fmt::format("x1     = {}\n", aObj.x1);
    // str += fmt::format("y1     = {}\n", aObj.y1);

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicCommentTextInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGRAPHICCOMMENTTEXTINST_HPP