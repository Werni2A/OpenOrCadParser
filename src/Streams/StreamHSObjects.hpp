#ifndef STREAMHSOBJECTS_HPP
#define STREAMHSOBJECTS_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StreamHSObjects : public CommonBase
{
public:

    StreamHSObjects(ParserContext& aCtx) : CommonBase{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


[[maybe_unused]]
static std::string to_string(const StreamHSObjects& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StreamHSObjects::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamHSObjects& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMHSOBJECTS_HPP