#ifndef STRUCTERCSYMBOL_HPP
#define STRUCTERCSYMBOL_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSymbol.hpp"
#include "Structures/StructSymbolBBox.hpp"


class StructERCSymbol : public StructSymbol
{
public:

    StructERCSymbol(StreamContext& aCtx) : StructSymbol{aCtx}, symbolBBox{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    StructSymbolBBox symbolBBox;
};


[[maybe_unused]]
static std::string to_string(const StructERCSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name     = {}\n", indent(1), aObj.name);
    str += fmt::format("{}someStr0 = {}\n", indent(1), aObj.someStr0);
    str += fmt::format("{}color    = {}\n", indent(1), ::to_string(aObj.color));
    str += fmt::format("{}symbolBBox = {}", indent(1), aObj.symbolBBox.to_string());

    return str;
}


inline std::string StructERCSymbol::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructERCSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTERCSYMBOL_HPP