#ifndef STREAMERC_HPP
#define STREAMERC_HPP


#include <memory>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"
#include "Structures/StructERCSymbol.hpp"


class StreamERC : public CommonBase
{
public:

    StreamERC(ParserContext& aCtx) : CommonBase{aCtx}, ercSymbol{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    std::unique_ptr<StructERCSymbol> ercSymbol;
};


[[maybe_unused]]
static std::string to_string(const StreamERC& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}ercSymbol:\n", indent(1));
    if(aObj.ercSymbol)
    {
        str += indent(aObj.ercSymbol->to_string(), 2);
    }

    return str;
}


inline std::string StreamERC::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamERC& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMERC_HPP