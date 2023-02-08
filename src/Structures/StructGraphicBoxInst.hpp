#ifndef STRUCTGRAPHICBOXINST_HPP
#define STRUCTGRAPHICBOXINST_HPP


#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "General.hpp"
#include "Structures/StructGraphicInst.hpp"
#include "Structures/StructSthInPages0.hpp"


class StructGraphicBoxInst : public StructGraphicInst
{
public:

    StructGraphicBoxInst(ParserContext& aCtx) : StructGraphicInst{aCtx}, dbId{0},
    locX{0}, locY{0}, x1{0}, y1{0}, x2{0}, y2{0}, color{Color::Default}, sthInPages0{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    uint32_t dbId;

    int16_t locX;
    int16_t locY;

    int16_t x1;
    int16_t y1;

    int16_t x2;
    int16_t y2;

    Color color;

    std::unique_ptr<StructSthInPages0> sthInPages0;
};


[[maybe_unused]]
static std::string to_string(const StructGraphicBoxInst& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}dbId  = {}\n", indent(1), aObj.dbId);
    str += fmt::format("{}locX  = {}\n", indent(1), aObj.locX);
    str += fmt::format("{}locY  = {}\n", indent(1), aObj.locY);
    str += fmt::format("{}x1    = {}\n", indent(1), aObj.x1);
    str += fmt::format("{}y1    = {}\n", indent(1), aObj.y1);
    str += fmt::format("{}x2    = {}\n", indent(1), aObj.x2);
    str += fmt::format("{}y2    = {}\n", indent(1), aObj.y2);
    str += fmt::format("{}color = {}\n", indent(1), to_string(aObj.color));

    str += fmt::format("{}sthInPages0:\n", indent(1));
    if(aObj.sthInPages0)
    {
        str += indent(aObj.sthInPages0->to_string(), 2);
    }

    return str;
}


inline std::string StructGraphicBoxInst::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicBoxInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGRAPHICBOXINST_HPP