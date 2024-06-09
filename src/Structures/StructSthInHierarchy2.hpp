#ifndef STRUCTSTHINHIERARCHY2_HPP
#define STRUCTSTHINHIERARCHY2_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "StructSomeHierarchyBase.hpp"


class StructSthInHierarchy2 : public StructSomeHierarchyBase
{
public:

    StructSthInHierarchy2(StreamContext& aCtx) : StructSomeHierarchyBase{aCtx}
    { }

    std::string to_string() const override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    Structure getObjectType() const override
    {
        return Structure::SthInHierarchy2;
    }

};


[[maybe_unused]]
static std::string to_string(const StructSthInHierarchy2& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructSthInHierarchy2::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSthInHierarchy2& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSTHINHIERARCHY2_HPP