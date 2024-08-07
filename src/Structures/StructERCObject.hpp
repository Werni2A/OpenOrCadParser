#ifndef STRUCTERCOBJECT_HPP
#define STRUCTERCOBJECT_HPP

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
class StructERCObject : public StructGraphicInst
{
public:
    StructERCObject(StreamContext& aCtx)
        : StructGraphicInst{aCtx},
          s0{},
          s1{},
          s2{}
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
        return Structure::ERCObject;
    }

    std::string s0;
    std::string s1;
    std::string s2;
};

[[maybe_unused]]
static std::string to_string(const StructERCObject& aObj)
{
    return aObj.to_string();
}

inline std::string StructERCObject::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    str += fmt::format("{}s0 = {}\n", indent(1), s0);
    str += fmt::format("{}s1 = {}\n", indent(1), s1);
    str += fmt::format("{}s2 = {}\n", indent(1), s2);

    return str;
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructERCObject& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTERCOBJECT_HPP