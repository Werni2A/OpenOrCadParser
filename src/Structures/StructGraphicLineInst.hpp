#ifndef STRUCTGRAPHICLINEINST_HPP
#define STRUCTGRAPHICLINEINST_HPP


#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"
#include "Structures/StructSthInPages0.hpp"


class StructGraphicLineInst : public CommonBase
{
public:

    StructGraphicLineInst(DataStream& aDs) : CommonBase{aDs}, sthInPages0{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::unique_ptr<StructSthInPages0> sthInPages0;
};


[[maybe_unused]]
static std::string to_string(const StructGraphicLineInst& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}sthInPages0:\n", indent(1));
    str += indent(aObj.sthInPages0->to_string(), 2);

    return str;
}


inline std::string StructGraphicLineInst::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicLineInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGRAPHICLINEINST_HPP