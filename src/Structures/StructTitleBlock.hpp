#ifndef STRUCTTITLEBLOCK_HPP
#define STRUCTTITLEBLOCK_HPP

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
class StructTitleBlock : public StructGraphicInst
{
public:
    StructTitleBlock(StreamContext& aCtx)
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
        return Structure::TitleBlock;
    }
};

[[maybe_unused]] static std::string to_string(const StructTitleBlock& aObj)
{
    return aObj.to_string();
}

inline std::string StructTitleBlock::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    return str;
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructTitleBlock& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP
#endif // STRUCTTITLEBLOCK_HPP