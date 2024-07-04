#ifndef STRUCTT0X10_HPP
#define STRUCTT0X10_HPP

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

namespace OOCP
{
class StructT0x10 : public Record
{
public:
    StructT0x10(StreamContext& aCtx)
        : Record{aCtx},
          symbolDisplayProps{}
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
        return Structure::T0x10;
    }

    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
};

[[maybe_unused]] static std::string to_string(const StructT0x10& aObj)
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

    return str;
}

inline std::string StructT0x10::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructT0x10& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTT0X10_HPP