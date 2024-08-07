#ifndef STRUCTPORTSYMBOL_HPP
#define STRUCTPORTSYMBOL_HPP

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSymbol.hpp"

namespace OOCP
{
class StructPortSymbol : public StructSymbol
{
public:
    StructPortSymbol(StreamContext& aCtx)
        : StructSymbol{aCtx}
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
        return Structure::PortSymbol;
    }
};

[[maybe_unused]]
static std::string to_string(const StructPortSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}

inline std::string StructPortSymbol::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPortSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTPORTSYMBOL_HPP