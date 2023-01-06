#ifndef STRUCTPROPERTIES_HPP
#define STRUCTPROPERTIES_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"


class StructProperties : public CommonBase
{
public:

    StructProperties(DataStream& aDs) : CommonBase{aDs}, ref{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string ref;
};


[[maybe_unused]]
static std::string to_string(const StructProperties& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}ref  = {}\n", indent(1), aObj.ref);

    return str;
}


inline std::string StructProperties::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructProperties& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPROPERTIES_HPP