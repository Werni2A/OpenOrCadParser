



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
static std::string to_string(const PinIdxMapping& pinIdxMapping)
{
    std::string str;

    str += "PinIdxMapping:" + newLine();
    str += indent(1) + "unitRef = " + pinIdxMapping.unitRef + newLine();
    str += indent(1) + "refDes  = " + pinIdxMapping.refDes  + newLine();

    str += indent(1) + "pinMap:" + newLine();
    for(size_t i = 0u; i < pinIdxMapping.pinMap.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + pinIdxMapping.pinMap[i], 2) + newLine();
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const PinIdxMapping& pinIdxMapping)
{
    os << to_string(pinIdxMapping);

    return os;
}


#endif // PINIDXMAPPING_H