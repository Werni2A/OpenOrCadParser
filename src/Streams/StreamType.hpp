#ifndef STREAMTYPE_HPP
#define STREAMTYPE_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/ComponentType.hpp"


// @note This is used in `$Type$` files
class Type
{
public:

    Type() : name{}, componentType{ComponentType::Graphic}
    { }

    std::string to_string() const;

    std::string name;
    ComponentType componentType;
};


[[maybe_unused]]
static std::string to_string(const Type& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}componentType = {}\n", indent(1), to_string(aObj.componentType));

    return str;
}


inline std::string Type::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Type& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


class StreamType : public CommonBase
{
public:

    StreamType(DataStream& aDs) : CommonBase{aDs}, types{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::vector<Type> types;
};


[[maybe_unused]]
static std::string to_string(const StreamType& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}types:\n", indent(1));
    for(size_t i = 0u; i < aObj.types.size(); ++i)
    {
        str += fmt::format("[{:>3}]:\n", i);
        str += indent(aObj.types[i].to_string(), 2);
    }

    return str;
}


inline std::string StreamType::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamType& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STREAMTYPE_HPP