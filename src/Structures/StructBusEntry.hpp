#ifndef STRUCTBUSENTRY_HPP
#define STRUCTBUSENTRY_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "General.hpp"


class StructBusEntry : public CommonBase
{
public:

    StructBusEntry(ParserContext& aCtx) : CommonBase{aCtx}, color{Color::Default},
        startX{0}, startY{0}, endX{0}, endY{0}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    Color    color;

    int32_t  startX;
    int32_t  startY;

    int32_t  endX;
    int32_t  endY;
};


[[maybe_unused]]
static std::string to_string(const StructBusEntry& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}color  = {}\n", indent(1), to_string(aObj.color));
    str += fmt::format("{}startX = {}\n", indent(1), aObj.startX);
    str += fmt::format("{}startY = {}\n", indent(1), aObj.startY);
    str += fmt::format("{}endX   = {}\n", indent(1), aObj.endX);
    str += fmt::format("{}endY   = {}\n", indent(1), aObj.endY);

    return str;
}


inline std::string StructBusEntry::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructBusEntry& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTBUSENTRY_HPP