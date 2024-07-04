#ifndef STREAMHSOBJECTS_HPP
#define STREAMHSOBJECTS_HPP

#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"

class StreamHSObjects : public Stream
{
public:
    StreamHSObjects(ContainerContext& aCtx, const fs::path& aInputStream)
        : Stream{aCtx, aInputStream}
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
        return StreamType::HSObjects;
    }
};

[[maybe_unused]] static std::string to_string(const StreamHSObjects& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}

inline std::string StreamHSObjects::to_string() const
{
    return ::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StreamHSObjects& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}

#endif // STREAMHSOBJECTS_HPP