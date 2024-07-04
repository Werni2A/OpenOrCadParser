#ifndef STREAMERC_HPP
#define STREAMERC_HPP

#include <memory>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"
#include "Structures/StructERCSymbol.hpp"

namespace OOCP
{
class StreamERC : public Stream
{
public:
    StreamERC(ContainerContext& aCtx, const fs::path& aInputStream)
        : Stream{aCtx, aInputStream},
          ercSymbol{}
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
        return StreamType::ERC;
    }

    std::unique_ptr<StructERCSymbol> ercSymbol;
};

[[maybe_unused]] static std::string to_string(const StreamERC& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}ercSymbol:\n", indent(1));
    if(aObj.ercSymbol)
    {
        str += indent(aObj.ercSymbol->to_string(), 2);
    }

    return str;
}

inline std::string StreamERC::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StreamERC& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // STREAMERC_HPP