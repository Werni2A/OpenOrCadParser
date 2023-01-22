#ifndef STRUCTT0X10_HPP
#define STRUCTT0X10_HPP


#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"


class StructT0x10 : public CommonBase
{
public:

    StructT0x10(DataStream& aDs) : CommonBase{aDs}, symbolDisplayProps{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
};


[[maybe_unused]]
static std::string to_string(const StructT0x10& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.symbolDisplayProps[i]->to_string()), 2);
    }

    return str;
}


inline std::string StructT0x10::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructT0x10& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTT0X10_HPP