#ifndef STREAMDTYPED_HPP
#define STREAMDTYPED_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/ComponentType.hpp"
#include "Stream.hpp"

namespace OOCP
{
// @note This is used in `$Type$` files
class Type
{
public:
    Type()
        : name{},
          componentType{ComponentType::Graphic}
    {
    }

    std::string to_string() const;

    std::string name;
    ComponentType componentType;
};

[[maybe_unused]] static std::string to_string(const Type& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);
    str += fmt::format("{}componentType = {}\n", indent(1), to_string(aObj.componentType));

    return str;
}

inline std::string Type::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const Type& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}

class StreamDTypeD : public Stream
{
public:
    StreamDTypeD(ContainerContext& aCtx, const fs::path& aInputStream)
        : Stream{aCtx, aInputStream},
          types{}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    virtual StreamType getStreamType() const override
    {
        return StreamType::DTypeD;
    }

    std::vector<Type> types;
};

[[maybe_unused]] static std::string to_string(const StreamDTypeD& aObj)
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

inline std::string StreamDTypeD::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StreamDTypeD& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP
#endif // STREAMDTYPED_HPP