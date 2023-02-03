#ifndef STRUCTSYMBOL_HPP
#define STRUCTSYMBOL_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructSthInPages0.hpp"


/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructSymbol : public StructSthInPages0
{
public:

    StructSymbol(ParserContext& aCtx) : StructSthInPages0{aCtx}
    { }

    std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;
};


[[maybe_unused]]
static std::string to_string(const StructSymbol& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructSymbol::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSymbol& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTSYMBOL_HPP