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


class Database
{
public:

    Database() : mStreams{}
    { }

    ~Database()
    { }

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    std::optional<std::shared_ptr<Stream>> getLibrary() const
    {
        for(const auto& stream : mStreams)
        {
            if(stream->mCtx.mCfbfStreamLocation.matches_pattern({"Library"}))
            {
                return std::make_optional(stream);
            }
        }

        return std::nullopt;
    }

    std::optional<std::shared_ptr<Stream>> getDirectory(DirectoryType aDirType) const
    {
        for(const auto& stream : mStreams)
        {
            if(stream->mCtx.mCfbfStreamLocation.matches_pattern({to_string(aDirType)}))
            {
                return std::make_optional(stream);
            }
        }

        return std::nullopt;
    }

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


#endif // DATABASE_HPP