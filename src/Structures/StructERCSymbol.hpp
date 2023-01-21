#ifndef STRUCTERCSYMBOL_HPP
#define STRUCTERCSYMBOL_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"
#include "Structures/StructSymbolBBox.hpp"


class StructERCSymbol : public CommonBase
{
public:

    StructERCSymbol(DataStream& aDs) : CommonBase{aDs}, name{}, symbolBBox{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string name;

    StructSymbolBBox symbolBBox;
};


[[maybe_unused]]
static std::string to_string(const StructERCSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}symbolBBox = {}", indent(1), to_string(aObj.symbolBBox));

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