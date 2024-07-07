#ifndef STREAMDSNSTREAM_HPP
#define STREAMDSNSTREAM_HPP

#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"

namespace OOCP
{
class StreamDsnStream : public Stream
{
public:
    StreamDsnStream(ContainerContext& aCtx, const fs::path& aInputStream)
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
        return StreamType::DsnStream;
    }
};

[[maybe_unused]]
static std::string to_string(const StreamDsnStream& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}

inline std::string StreamDsnStream::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamDsnStream& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // STREAMDSNSTREAM_HPP