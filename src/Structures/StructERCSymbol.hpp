#ifndef STRUCTERCSYMBOL_HPP
#define STRUCTERCSYMBOL_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSymbolBBox.hpp"


struct StructERCSymbol
{
    std::string name;

    StructSymbolBBox symbolBBox;
};


[[maybe_unused]]
static std::string to_string(const StructERCSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("name = {}\n", aObj.name);
    str += fmt::format("symbolBBox = {}", to_string(aObj.symbolBBox));

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructERCSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTERCSYMBOL_HPP