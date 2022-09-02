#ifndef STREAMPACKAGE_HPP
#define STREAMPACKAGE_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "DataVariants.hpp"


struct StreamPackage
{
    std::vector<VariantStructure> structures;
};


[[maybe_unused]]
static std::string to_string(const StreamPackage& aObj)
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
static std::ostream& operator<<(std::ostream& aOs, const StreamPackage& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMPACKAGE_HPP