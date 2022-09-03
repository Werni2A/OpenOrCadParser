



#ifndef STRUCTPINIDXMAPPING_HPP
#define STRUCTPINIDXMAPPING_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>


// @todo Probably called 'PhysicalPart' in the XSD
//       Structure should also contain 'PinShared' and 'PinSwap'
struct StructPinIdxMapping
{
    std::string unitRef;
    std::string refDes;

    // @todo Probably called 'PinNumber' in the XSD where
    //       the index in the vector equals the position and
    //       numer equals the string value in the vector.
    std::vector<std::string> pinMap;
};


[[maybe_unused]]
static std::string to_string(const StructPinIdxMapping& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}unitRef = {}\n", indent(1), aObj.unitRef);
    str += fmt::format("{}refDes  = {}\n", indent(1), aObj.refDes);

    str += fmt::format("{}pinMap:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinMap.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}\n", i, aObj.pinMap[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPinIdxMapping& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPINIDXMAPPING_HPP