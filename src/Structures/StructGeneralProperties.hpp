#ifndef STRUCTGENERALPROPERTIES_HPP
#define STRUCTGENERALPROPERTIES_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/ImplementationType.hpp"


class StructGeneralProperties : public CommonBase
{
public:

    StructGeneralProperties(DataStream& aDs) : CommonBase{aDs}, implementationPath{},
        implementation{}, refDes{}, partValue{}, implementationType{ImplementationType::None},
        pinNameVisible{true}, pinNameRotate{false}, pinNumberVisible{true}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string implementationPath;
    std::string implementation;
    std::string refDes;
    std::string partValue;

    ImplementationType implementationType;

    bool pinNameVisible;
    bool pinNameRotate;
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