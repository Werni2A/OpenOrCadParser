#ifndef STRUCTT0X45_HPP
#define STRUCTT0X45_HPP

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "StructSomeHierarchyBase.hpp"

namespace OOCP
{
class StructT0x45 : public StructSomeHierarchyBase
{
public:
    StructT0x45(StreamContext& aCtx)
        : StructSomeHierarchyBase{aCtx}
    {
    }

    std::string to_string() const override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    Structure getObjectType() const override
    {
        return Structure::T0x45;
    }
};

[[maybe_unused]]
static std::string to_string(const StructT0x45& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}

inline std::string StructT0x45::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructT0x45& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTT0X45_HPP