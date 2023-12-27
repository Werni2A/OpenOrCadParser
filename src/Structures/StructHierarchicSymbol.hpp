#ifndef STRUCTHIERARCHICSYMBOL_HPP
#define STRUCTHIERARCHICSYMBOL_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"


class StructHierarchicSymbol : public Record
{
public:

    StructHierarchicSymbol(StreamContext& aCtx) : Record{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


[[maybe_unused]]
static std::string to_string(const StructHierarchicSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructHierarchicSymbol::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructHierarchicSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTHIERARCHICSYMBOL_HPP