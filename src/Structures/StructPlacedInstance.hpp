#ifndef STRUCTPLACEDINSTANCE_HPP
#define STRUCTPLACEDINSTANCE_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructT0x10.hpp"

class StructPlacedInstance : public Record
{
public:
    StructPlacedInstance(StreamContext& aCtx)
        : Record{aCtx},
          symbolDisplayProps{},
          t0x10s{}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Structure getObjectType() const override
    {
        return Structure::PlacedInstance;
    }

    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
    std::vector<std::unique_ptr<StructT0x10>> t0x10s;
};

[[maybe_unused]] static std::string to_string(const StructPlacedInstance& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        if(aObj.symbolDisplayProps[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.symbolDisplayProps[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}t0x10s:\n", indent(1));
    for(size_t i = 0u; i < aObj.t0x10s.size(); ++i)
    {
        if(aObj.t0x10s[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.t0x10s[i]->to_string()), 2);
        }
    }

    return str;
}

inline std::string StructPlacedInstance::to_string() const
{
    return ::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructPlacedInstance& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}

#endif // STRUCTPLACEDINSTANCE_HPP