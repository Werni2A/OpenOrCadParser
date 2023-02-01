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
#include "Structures/StructT0x1f.hpp"


class StreamPackage : public CommonBase
{
public:

    StreamPackage(ParserContext& aCtx) : CommonBase{aCtx}, properties{}, primitives{},
        t0x1f{}, pinIdxMappings{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::vector<std::unique_ptr<StructProperties>>    properties;
    std::vector<std::unique_ptr<StructPrimitives>>    primitives;

    std::unique_ptr<StructT0x1f>                      t0x1f;

    std::vector<std::unique_ptr<StructPinIdxMapping>> pinIdxMappings;
};


[[maybe_unused]]
static std::string to_string(const StreamPackage& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}properties:\n", indent(1));
    for(size_t i = 0u; i < aObj.properties.size(); ++i)
    {
        if(aObj.properties[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.properties[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}primitives:\n", indent(1));
    for(size_t i = 0u; i < aObj.primitives.size(); ++i)
    {
        if(aObj.primitives[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.primitives[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}t0x1f:\n", indent(1));
    if(aObj.t0x1f)
    {
        str += indent(aObj.t0x1f->to_string(), 2);
    }

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