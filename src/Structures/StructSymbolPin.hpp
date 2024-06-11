#ifndef STRUCTSYMBOLPIN_HPP
#define STRUCTSYMBOLPIN_HPP


#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/PortType.hpp"
#include "PinShape.hpp"
#include "Record.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"


/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructSymbolPin : public Record
{
public:

    StructSymbolPin(StreamContext& aCtx) : Record{aCtx}, name{}, startX{0}, startY{0},
        hotptX{0}, hotptY{0}, pinShape{}, portType{PortType::Input}, symbolDisplayProps{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    int32_t getPinLength() const;

    std::string name;
    int32_t startX;
    int32_t startY;
    int32_t hotptX;
    int32_t hotptY;

    PinShape pinShape;

    PortType portType;

    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;
};


[[maybe_unused]]
static std::string to_string(const StructSymbolPin& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name     = {}\n", indent(1), aObj.name);
    str += fmt::format("{}startX   = {}\n", indent(1), aObj.startX);
    str += fmt::format("{}startY   = {}\n", indent(1), aObj.startY);
    str += fmt::format("{}hotptX   = {}\n", indent(1), aObj.hotptX);
    str += fmt::format("{}hotptY   = {}\n", indent(1), aObj.hotptY);

    str += fmt::format("{}pinShape:\n", indent(1));
    str += indent(to_string(aObj.pinShape), 2);

    str += fmt::format("{}portType = {}\n", indent(1), to_string(aObj.portType));

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        if(aObj.symbolDisplayProps[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.symbolDisplayProps[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StructSymbolPin::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSymbolPin& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTSYMBOLPIN_HPP