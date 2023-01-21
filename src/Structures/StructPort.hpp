#ifndef STRUCTPORT_HPP
#define STRUCTPORT_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"


class StructPort : public CommonBase
{
public:

    StructPort(DataStream& aDs) : CommonBase{aDs}, name{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string name;
};


[[maybe_unused]]
static std::string to_string(const StructPort& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);

    return str;
}


inline std::string StructPort::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPort& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTPORT_HPP