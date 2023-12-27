#ifndef STREAMSCHEMATIC_HPP
#define STREAMSCHEMATIC_HPP


#include <memory>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"


class StreamSchematic : public Stream
{
public:

    StreamSchematic(ContainerContext& aCtx, const fs::path& aInputStream) : Stream{aCtx, aInputStream}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


[[maybe_unused]]
static std::string to_string(const StreamSchematic& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StreamSchematic::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamSchematic& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMSCHEMATIC_HPP