#ifndef STRUCTSTHINHIERARCHY3_HPP
#define STRUCTSTHINHIERARCHY3_HPP

#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"

class StructSthInHierarchy3 : public Record
{
public:
    StructSthInHierarchy3(StreamContext& aCtx)
        : Record{aCtx}
    {
    }

    std::string to_string() const override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    Structure getObjectType() const override
    {
        return Structure::SthInHierarchy3;
    }
};

[[maybe_unused]] static std::string to_string(const StructSthInHierarchy3& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}

inline std::string StructSthInHierarchy3::to_string() const
{
    return ::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructSthInHierarchy3& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}

#endif // STRUCTSTHINHIERARCHY3_HPP