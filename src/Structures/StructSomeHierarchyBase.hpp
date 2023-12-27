#ifndef STRUCTSOMEHIERARCHYBASE_HPP
#define STRUCTSOMEHIERARCHYBASE_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"


/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructSomeHierarchyBase : public Record
{
public:

    StructSomeHierarchyBase(StreamContext& aCtx) : Record{aCtx}
    { }

    // std::string to_string() const override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructSomeHierarchyBase& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSomeHierarchyBase& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSOMEHIERARCHYBASE_HPP