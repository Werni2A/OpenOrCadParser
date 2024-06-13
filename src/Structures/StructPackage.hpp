#ifndef STRUCTPACKAGE_HPP
#define STRUCTPACKAGE_HPP


#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Record.hpp"
#include "Structures/StructDevice.hpp"

class StructPackage : public Record
{
public:

    StructPackage(StreamContext& aCtx) : Record{aCtx}, name{},
        refDes{}, pcbFootprint{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Structure getObjectType() const override
    {
        return Structure::Package;
    }

    std::string name;
    std::string refDes;

    // OrCAD GUI: `Property Sheet` -> `Package Properties` -> `PCB Footprint`
    std::string pcbFootprint;

    std::vector<std::unique_ptr<StructDevice>> devices;
};


[[maybe_unused]]
static std::string to_string(const StructPackage& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name   = {}\n", indent(1), aObj.name);
    str += fmt::format("{}refDes = {}\n", indent(1), aObj.refDes);
    str += fmt::format("{}pcbFootprint = {}\n", indent(1), aObj.pcbFootprint);

    str += fmt::format("{}devices:\n", indent(1));
    for(size_t i = 0u; i < aObj.devices.size(); ++i)
    {
        if(aObj.devices[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.devices[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StructPackage::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPackage& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPACKAGE_HPP