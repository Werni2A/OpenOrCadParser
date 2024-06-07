#ifndef STRUCTSTHINHIERARCHY1_HPP
#define STRUCTSTHINHIERARCHY1_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"


class StructSthInHierarchy1 : public Record
{
public:

    StructSthInHierarchy1(StreamContext& aCtx) : Record{aCtx}
    { }

    std::string to_string() const override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructSthInHierarchy1& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructSthInHierarchy1::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSthInHierarchy1& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSTHINHIERARCHY1_HPP