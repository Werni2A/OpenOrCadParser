#ifndef STREAMBOMDATASTREAM_HPP
#define STREAMBOMDATASTREAM_HPP

#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"

class StreamBOMDataStream : public Stream
{
public:
    StreamBOMDataStream(ContainerContext& aCtx, const fs::path& aInputStream)
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
        return StreamType::BOMDataStream;
    }
};

[[maybe_unused]] static std::string to_string(const StreamBOMDataStream& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}

inline std::string StreamBOMDataStream::to_string() const
{
    return ::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StreamBOMDataStream& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}

#endif // STREAMBOMDATASTREAM_HPP