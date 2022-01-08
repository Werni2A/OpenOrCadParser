



#ifndef PINIDXMAPPING_H
#define PINIDXMAPPING_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>


// @todo Probably called 'PhysicalPart' in the XSD
//       Structure should also contain 'PinShared' and 'PinSwap'
struct PinIdxMapping
{
    std::string unitRef;
    std::string refDes;

    // @todo Probably called 'PinNumber' in the XSD where
    //       the index in the vector equals the position and
    //       numer equals the string value in the vector.
    std::vector<std::string> pinMap;
};


[[maybe_unused]]
static std::string to_string(const PinIdxMapping& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "unitRef = " + aObj.unitRef + newLine();
    str += indent(1) + "refDes  = " + aObj.refDes  + newLine();

    str += indent(1) + "pinMap:" + newLine();
    for(size_t i = 0u; i < aObj.pinMap.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + aObj.pinMap[i], 2) + newLine();
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PinIdxMapping& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PINIDXMAPPING_H