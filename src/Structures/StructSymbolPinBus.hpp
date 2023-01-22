#ifndef STRUCTSYMBOLPINBUS_HPP
#define STRUCTSYMBOLPINBUS_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/PortType.hpp"
#include "PinShape.hpp"
#include "Structures/StructSymbolPin.hpp"


// @todo this looks exactly the same as SymbolPinScalar. Why is that?
class StructSymbolPinBus : public StructSymbolPin
{
public:

    StructSymbolPinBus(DataStream& aDs) : StructSymbolPin{aDs}, name{}, startX{0}, startY{0},
        hotptX{0}, hotptY{0}, pinShape{}, portType{PortType::Input}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string name;
    int32_t startX;
    int32_t startY;
    int32_t hotptX;
    int32_t hotptY;

    PinShape pinShape;

    PortType portType;
};


[[maybe_unused]]
static std::string to_string(const StructSymbolPinBus& aObj)
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

    return str;
}


inline std::string StructSymbolPinBus::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructSymbolPinBus& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTSYMBOLPINBUS_HPP