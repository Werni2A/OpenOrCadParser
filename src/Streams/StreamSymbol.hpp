#ifndef STREAMSYMBOL_HPP
#define STREAMSYMBOL_HPP

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"
#include "Structures/StructSymbol.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPin.hpp"

namespace OOCP
{
class StreamSymbol : public Stream
{
public:
    StreamSymbol(ContainerContext& aCtx, const fs::path& aInputStream)
        : Stream{aCtx, aInputStream},
          symbolPins{},
          symbolDisplayProps{}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    virtual StreamType getStreamType() const override
    {
        return StreamType::Symbol;
    }

    std::unique_ptr<StructSymbol> symbol;

    std::vector<std::unique_ptr<StructSymbolPin>> symbolPins;
    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
};

[[maybe_unused]]
static std::string to_string(const StreamSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += "symbol:\n";
    if(aObj.symbol)
    {
        str += indent(aObj.symbol->to_string(), 2);
    }

    str += fmt::format("{}symbolPins:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolPins.size(); ++i)
    {
        if(aObj.symbolPins[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.symbolPins[i]->to_string()), 2);
        }
    }

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

inline std::string StreamSymbol::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // STREAMSYMBOL_HPP