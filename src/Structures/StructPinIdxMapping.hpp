



#ifndef STRUCTPINIDXMAPPING_HPP
#define STRUCTPINIDXMAPPING_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Record.hpp"


// @todo Probably called 'PhysicalPart' in the XSD
//       Structure should also contain 'PinShared' and 'PinSwap'
class StructPinIdxMapping : public Record
{
public:

    StructPinIdxMapping(StreamContext& aCtx) : Record{aCtx}, unitRef{}, refDes{},
        pinMap{}, pinIgnore{}, pinGroup{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    std::string unitRef;
    std::string refDes;

    // @todo Probably called 'PinNumber' in the XSD where
    //       the index in the vector equals the position and
    //       numer equals the string value in the vector.
    std::vector<std::string> pinMap;

    // @todo add to a separate pin struct
    std::vector<bool>        pinIgnore;
    std::vector<uint8_t>     pinGroup; // @todo Probably convert to std::optional<uint8_t>
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

    str += fmt::format("{}pinIgnore:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinIgnore.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}\n", i, aObj.pinIgnore[i]), 2);
    }

    str += fmt::format("{}pinGroup:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinGroup.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}\n", i, aObj.pinGroup[i]), 2);
    }

    return str;
}


inline std::string StructPinIdxMapping::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPinIdxMapping& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPINIDXMAPPING_HPP