#ifndef STRUCTSTHINPAGES0_HPP
#define STRUCTSTHINPAGES0_HPP


#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "General.hpp"
#include "Primitives/PrimBase.hpp"


class StructSthInPages0 : public CommonBase
{
public:

    StructSthInPages0(ParserContext& aCtx) : CommonBase{aCtx},
        color{Color::Default}, primitives{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    Color color;

    std::vector<std::unique_ptr<PrimBase>> primitives;
};


[[maybe_unused]]
static std::string to_string(const StructSthInPages0& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}color = {}\n", indent(1), ::to_string(aObj.color));

    str += fmt::format("{}primitives:\n", indent(1));
    for(size_t i = 0u; i < aObj.primitives.size(); ++i)
    {
        if(aObj.primitives[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.primitives[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StructSthInPages0::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSthInPages0& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSTHINPAGES0_HPP