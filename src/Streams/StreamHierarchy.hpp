#ifndef STREAMHIERARCHY_HPP
#define STREAMHIERARCHY_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StreamHierarchy : public CommonBase
{
public:

    StreamHierarchy(ParserContext& aCtx) : CommonBase{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


[[maybe_unused]]
static std::string to_string(const StreamHierarchy& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StreamHierarchy::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamHierarchy& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STREAMHIERARCHY_HPP