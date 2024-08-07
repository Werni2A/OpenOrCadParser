#ifndef STRUCTALIAS_HPP
#define STRUCTALIAS_HPP

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "Enums/Rotation.hpp"
#include "General.hpp"
#include "Record.hpp"

namespace OOCP
{
class StructAlias : public Record
{
public:
    StructAlias(StreamContext& aCtx)
        : Record{aCtx},
          locX{0},
          locY{0},
          color{Color::Default},
          rotation{Rotation::Deg_0},
          name{}
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
        return Structure::Alias;
    }

    int32_t locX;
    int32_t locY;

    Color color;
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
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructAlias& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTALIAS_HPP