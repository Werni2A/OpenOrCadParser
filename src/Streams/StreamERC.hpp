#ifndef STREAMERC_HPP
#define STREAMERC_HPP


#include <memory>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StreamERC : public CommonBase
{
public:

    StreamERC(ParserContext& aCtx) : CommonBase{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StreamERC& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StreamERC::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamERC& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMERC_HPP