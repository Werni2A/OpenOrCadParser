#ifndef STRUCTWIREBUS_HPP
#define STRUCTWIREBUS_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructWire.hpp"


class StructWireBus : public StructWire
{
public:

    StructWireBus(StreamContext& aCtx) : StructWire{aCtx}
    { }

    // std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    // uint32_t  id;
    // Color     color;

    // int32_t   startX;
    // int32_t   startY;
    // int32_t   endX;
    // int32_t   endY;

    // LineWidth lineWidth;
    // LineStyle lineStyle;
};


#endif // STRUCTWIREBUS_HPP