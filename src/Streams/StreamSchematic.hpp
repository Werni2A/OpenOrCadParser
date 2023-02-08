#ifndef STREAMSCHEMATIC_HPP
#define STREAMSCHEMATIC_HPP


#include <memory>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StreamSchematic : public CommonBase
{
public:

    StreamSchematic(ParserContext& aCtx) : CommonBase{aCtx}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
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