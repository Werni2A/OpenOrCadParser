#ifndef STREAMPACKAGE_HPP
#define STREAMPACKAGE_HPP


#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Structures/StructPinIdxMapping.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructProperties.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPin.hpp"
#include "Structures/StructT0x1f.hpp"


class StreamPackage : public CommonBase
{
public:

    StreamPackage(DataStream& aDs) : CommonBase{aDs}, properties{}, primitives{},
        symbolPins{}, symbolDisplayProps{}, t0x1f{}, pinIdxMappings{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::vector<std::unique_ptr<StructProperties>>        properties;
    std::vector<std::unique_ptr<StructPrimitives>>        primitives;
    std::vector<std::unique_ptr<StructSymbolPin>>         symbolPins;
    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;

    std::unique_ptr<StructT0x1f>                          t0x1f;

    std::vector<std::unique_ptr<StructPinIdxMapping>>     pinIdxMappings;
};


[[maybe_unused]]
static std::string to_string(const StreamPackage& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}properties:\n", indent(1));
    for(size_t i = 0u; i < aObj.properties.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.properties[i]->to_string()), 2);
    }

    str += fmt::format("{}primitives:\n", indent(1));
    for(size_t i = 0u; i < aObj.primitives.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.primitives[i]->to_string()), 2);
    }

    str += fmt::format("{}symbolPins:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolPins.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.symbolPins[i]->to_string()), 2);
    }

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.symbolDisplayProps[i]->to_string()), 2);
    }

    str += fmt::format("{}t0x1f:\n", indent(1));
    str += indent(aObj.t0x1f->to_string(), 2);

    str += fmt::format("{}pinIdxMappings:\n", indent(1));
    for(size_t i = 0u; i < aObj.pinIdxMappings.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.pinIdxMappings[i]->to_string()), 2);
    }

    return str;
}


inline std::string StreamPackage::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamPackage& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMPACKAGE_HPP