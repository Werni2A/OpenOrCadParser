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


class StructSymbolPinBus : public StructSymbolPin
{
public:

    StructSymbolPinBus(StreamContext& aCtx) : StructSymbolPin{aCtx}
    { }

    // std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }
};


#endif // STRUCTSYMBOLPINBUS_HPP