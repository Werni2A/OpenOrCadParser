#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/DirectoryType.hpp"
#include "Enums/StreamType.hpp"
#include "Stream.hpp"

namespace OOCP
{
class Database
{
public:
    Database()
        : mStreams{}
    {
    }

    ~Database()
    {
    }

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    // List of streams in the CFBF container
    std::vector<std::shared_ptr<Stream>> mStreams;
};

[[maybe_unused]]
static std::string to_string(const Database& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    for(const auto& stream : aObj.mStreams)
    {
        str += indent(stream->to_string(), 2);
    }

    return str;
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const Database& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // DATABASE_HPP