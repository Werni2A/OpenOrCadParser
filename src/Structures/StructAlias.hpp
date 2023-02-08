#ifndef STRUCTALIAS_HPP
#define STRUCTALIAS_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "Enums/Rotation.hpp"
#include "General.hpp"


class StructAlias : public CommonBase
{
public:

    StructAlias(ParserContext& aCtx) : CommonBase{aCtx}, locX{0}, locY{0},
        color{Color::Default}, rotation{Rotation::Deg_0}, name{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    int32_t  locX;
    int32_t  locY;

    Color    color;
    Rotation rotation;

    std::string name;
};


[[maybe_unused]]
static std::string to_string(const StructAlias& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}locX     = {}\n", indent(1), aObj.locX);
    str += fmt::format("{}locY     = {}\n", indent(1), aObj.locY);
    str += fmt::format("{}color    = {}\n", indent(1), to_string(aObj.color));
    str += fmt::format("{}rotation = {}\n", indent(1), to_string(aObj.rotation));
    str += fmt::format("{}name     = {}\n", indent(1), aObj.name);

    return str;
}


inline std::string StructAlias::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructAlias& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTALIAS_HPP