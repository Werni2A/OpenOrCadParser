#ifndef STRUCTGLOBALSYMBOL_HPP
#define STRUCTGLOBALSYMBOL_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSymbol.hpp"


class StructGlobalSymbol : public StructSymbol
{
public:

    StructGlobalSymbol(DataStream& aDs) : StructSymbol{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructGlobalSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

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