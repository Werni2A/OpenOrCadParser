#ifndef STRUCTPORT_HPP
#define STRUCTPORT_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructGraphicInst.hpp"

namespace OOCP
{
class StructPort : public StructGraphicInst
{
public:
    StructPort(StreamContext& aCtx)
        : StructGraphicInst{aCtx}
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
        return Structure::Port;
    }
};

[[maybe_unused]]
static std::string to_string(const StructPort& aObj)
{
    return aObj.to_string();
}

inline std::string StructPort::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    return str;
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPort& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTPORT_HPP