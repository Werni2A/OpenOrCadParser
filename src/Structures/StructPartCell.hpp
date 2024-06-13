#ifndef STRUCTPARTCELL_HPP
#define STRUCTPARTCELL_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Record.hpp"


class StructPartCell : public Record
{
public:

    StructPartCell(StreamContext& aCtx) : Record{aCtx}, ref{},
        normalName{}, convertName{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Structure getObjectType() const override
    {
        return Structure::PartCell;
    }

    std::string ref;
    std::string normalName;
    std::string convertName;
};


[[maybe_unused]]
static std::string to_string(const StructPartCell& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}ref = {}\n", indent(1), aObj.ref);
    str += fmt::format("{}normalName  = {}\n", indent(1), aObj.normalName);
    str += fmt::format("{}convertName = {}\n", indent(1), aObj.convertName);

    return str;
}


inline std::string StructPartCell::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructPartCell& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTPARTCELL_HPP