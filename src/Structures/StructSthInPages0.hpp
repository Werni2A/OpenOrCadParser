#ifndef STRUCTSTHINPAGES0_HPP
#define STRUCTSTHINPAGES0_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StructSthInPages0 : public CommonBase
{
public:

    StructSthInPages0(DataStream& aDs) : CommonBase{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructSthInPages0& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructSthInPages0::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSthInPages0& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSTHINPAGES0_HPP