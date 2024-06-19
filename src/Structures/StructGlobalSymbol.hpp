#ifndef STRUCTGLOBALSYMBOL_HPP
#define STRUCTGLOBALSYMBOL_HPP


#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSymbol.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"
#include "Structures/StructSymbolPin.hpp"


class StructGlobalSymbol : public StructSymbol
{
public:

    StructGlobalSymbol(StreamContext& aCtx) : StructSymbol{aCtx},
        symbolPins{}, symbolDisplayProps{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Structure getObjectType() const override
    {
        return Structure::GlobalSymbol;
    }

    std::vector<std::unique_ptr<StructSymbolPin>>         symbolPins;
    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
};


[[maybe_unused]]
static std::string to_string(const StructGlobalSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name          = {}\n", indent(1), aObj.name);
    str += fmt::format("{}sourceLibrary = {}\n", indent(1), aObj.sourceLibrary);
    str += fmt::format("{}color         = {}\n", indent(1), ::to_string(aObj.color));

    str += fmt::format("{}symbolPins:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolPins.size(); ++i)
    {
        if(aObj.symbolPins[i])
        {
            str += indent(fmt::format("[{}]: {}\n", i, aObj.symbolPins[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        if(aObj.symbolDisplayProps[i])
        {
            str += indent(fmt::format("[{}]: {}\n", i, aObj.symbolDisplayProps[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StructGlobalSymbol::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGlobalSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGLOBALSYMBOL_HPP