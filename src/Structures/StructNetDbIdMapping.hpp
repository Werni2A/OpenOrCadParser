#ifndef STRUCTNETDBIDMAPPING_HPP
#define STRUCTNETDBIDMAPPING_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StructNetDbIdMapping : public CommonBase
{
public:

    StructNetDbIdMapping(ParserContext& aCtx) : CommonBase{aCtx}
    { }

    std::string to_string() const override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructNetDbIdMapping& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructNetDbIdMapping::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructNetDbIdMapping& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTNETDBIDMAPPING_HPP