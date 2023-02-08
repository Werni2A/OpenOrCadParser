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

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"


class StructPort : public CommonBase
{
public:

    StructPort(ParserContext& aCtx) : CommonBase{aCtx}, name{}, symbolDisplayProps{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    std::string name;

    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
};


[[maybe_unused]]
static std::string to_string(const StructPort& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        if(aObj.symbolDisplayProps[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.symbolDisplayProps[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StructPort::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPort& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTPORT_HPP