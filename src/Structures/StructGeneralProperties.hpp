#ifndef STRUCTGENERALPROPERTIES_HPP
#define STRUCTGENERALPROPERTIES_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/ImplementationType.hpp"


// @todo This is just a helper class but not a real structure
//       Check if the same code is used multiple times, otherwise
//       Inline this structure?
class StructGeneralProperties : public CommonBase
{
public:

    StructGeneralProperties(ParserContext& aCtx) : CommonBase{aCtx}, implementationPath{},
        implementation{}, refDes{}, partValue{}, implementationType{ImplementationType::None},
        pinNameVisible{true}, pinNameRotate{false}, pinNumberVisible{true}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    // Implementation path to open the corresponding folder.
    // See OrCAD: 'Property Sheet' -> 'Part Properties' -> 'Implementation Path'
    std::string implementationPath;

    // Implementation of the symbol.
    // See OrCAD: 'Property Sheet' -> 'Part Properties' -> 'Implementation'
    std::string implementation;

    // Reference descriptor for the symbol. E.g. 'R' for resistor.
    // See OrCAD: 'Property Sheet' -> 'Package Properties' -> 'Part Reference Prefix'
    std::string refDes;

    // Value of the symbol. E.g. '10k' for a resistor.
    // See OrCAD: 'Property Sheet' -> 'Part Properties' -> 'Value'
    std::string partValue;

    // See OrCAD Dropdown Menu: 'Property Sheet' -> 'Part Properties' -> 'Implementation Type'
    ImplementationType implementationType;

    // See OrCAD Checkbox: 'Property Sheet' -> 'Part Properties' -> 'Pin Name Visible'
    bool pinNameVisible;

    // See OrCAD Checkbox: 'Property Sheet' -> 'Part Properties' -> 'Pin Name Rotate'
    bool pinNameRotate;

    // See OrCAD Checkbox: 'Property Sheet' -> 'Part Properties' -> 'Pin Number Visible'
    bool pinNumberVisible;
};


[[maybe_unused]]
static std::string to_string(const StructGeneralProperties& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}implementationPath = {}\n", indent(1), aObj.implementationPath);
    str += fmt::format("{}implementation     = {}\n", indent(1), aObj.implementation);
    str += fmt::format("{}refDes             = {}\n", indent(1), aObj.refDes);
    str += fmt::format("{}partValue          = {}\n", indent(1), aObj.partValue);
    str += fmt::format("{}implementationType = {}\n", indent(1), to_string(aObj.implementationType));
    str += fmt::format("{}pinNameVisible     = {}\n", indent(1), aObj.pinNameVisible);
    str += fmt::format("{}pinNameRotate      = {}\n", indent(1), aObj.pinNameRotate);
    str += fmt::format("{}pinNumberVisible   = {}\n", indent(1), aObj.pinNumberVisible);

    return str;
}


inline std::string StructGeneralProperties::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGeneralProperties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGENERALPROPERTIES_HPP