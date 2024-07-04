#ifndef STRUCTGRAPHICOLEEMBEDINST_HPP
#define STRUCTGRAPHICOLEEMBEDINST_HPP

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
class StructGraphicOleEmbedInst : public StructGraphicInst
{
public:
    StructGraphicOleEmbedInst(StreamContext& aCtx)
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
        return Structure::GraphicOleEmbedInst;
    }
};

[[maybe_unused]] static std::string to_string(const StructGraphicOleEmbedInst& aObj)
{
    return aObj.to_string();
}

inline std::string StructGraphicOleEmbedInst::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    return str;
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructGraphicOleEmbedInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // STRUCTGRAPHICOLEEMBEDINST_HPP