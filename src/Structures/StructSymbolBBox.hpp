#ifndef STRUCTSYMBOLBBOX_HPP
#define STRUCTSYMBOLBBOX_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"


class StructSymbolBBox : public Record
{
public:

    StructSymbolBBox(StreamContext& aCtx) : Record{aCtx}, x1{0}, y1{0}, x2{0}, y2{0}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    // @todo they are probably int16_t, see where those values are read.
    int32_t x1;
    int32_t y1;

    int32_t x2;
    int32_t y2;
};


[[maybe_unused]]
static std::string to_string(const StructSymbolBBox& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}x1 = {}\n", indent(1), aObj.x1);
    str += fmt::format("{}y1 = {}\n", indent(1), aObj.y1);
    str += fmt::format("{}x2 = {}\n", indent(1), aObj.x2);
    str += fmt::format("{}y2 = {}\n", indent(1), aObj.y2);

    return str;
}


inline std::string StructSymbolBBox::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSymbolBBox& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSYMBOLBBOX_HPP