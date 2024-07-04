#ifndef STRUCTGRAPHICLINEINST_HPP
#define STRUCTGRAPHICLINEINST_HPP

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
class StructGraphicLineInst : public StructGraphicInst
{
public:
    StructGraphicLineInst(StreamContext& aCtx)
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
        return Structure::GraphicLineInst;
    }
};

[[maybe_unused]] static std::string to_string(const StructGraphicLineInst& aObj)
{
    return aObj.to_string();
}

inline std::string StructGraphicLineInst::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    return str;
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructGraphicLineInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // STRUCTGRAPHICLINEINST_HPP