#ifndef STRUCTT0X1F_HPP
#define STRUCTT0X1F_HPP


#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Record.hpp"
#include "Structures/StructPinIdxMapping.hpp"

class StructT0x1f : public Record
{
public:

    StructT0x1f(StreamContext& aCtx) : Record{aCtx}, name{},
        refDes{}, pcbFootprint{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    std::string name;
    std::string refDes;

    // OrCAD GUI: `Property Sheet` -> `Package Properties` -> `PCB Footprint`
    std::string pcbFootprint;

    std::vector<std::unique_ptr<StructPinIdxMapping>> pinIdxMappings;
};


[[maybe_unused]]
static std::string to_string(const StructT0x1f& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name   = {}\n", indent(1), aObj.name);
    str += fmt::format("{}refDes = {}\n", indent(1), aObj.refDes);
    str += fmt::format("{}pcbFootprint = {}\n", indent(1), aObj.pcbFootprint);

    str += fmt::format("{}pinIdxMappings:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinIdxMappings.size(); ++i)
    {
        if(aObj.pinIdxMappings[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.pinIdxMappings[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StructT0x1f::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructT0x1f& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTT0X1F_HPP