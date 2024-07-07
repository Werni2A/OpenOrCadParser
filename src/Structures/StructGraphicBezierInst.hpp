#ifndef STRUCTGRAPHICBEZIERINST_HPP
#define STRUCTGRAPHICBEZIERINST_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructGraphicInst.hpp"

namespace OOCP
{
class StructGraphicBezierInst : public StructGraphicInst
{
public:
    StructGraphicBezierInst(StreamContext& aCtx)
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
        return Structure::GraphicBezierInst;
    }
};

[[maybe_unused]]
static std::string to_string(const StructGraphicBezierInst& aObj)
{
    return aObj.to_string();
}

inline std::string StructGraphicBezierInst::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    return str;
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicBezierInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTGRAPHICBEZIERINST_HPP