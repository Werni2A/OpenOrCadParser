#ifndef STREAMSYMBOL_HPP
#define STREAMSYMBOL_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "DataVariants.hpp"


struct StreamSymbol
{
    std::vector<VariantStructure> structures;
};


[[maybe_unused]]
static std::string to_string(const StreamSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}structures:\n", indent(1));
    for(size_t i = 0u; i < aObj.structures.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.structures[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMSYMBOL_HPP