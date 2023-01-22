#ifndef STRUCTWIRESCALAR_HPP
#define STRUCTWIRESCALAR_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructWire.hpp"


class StructWireScalar : public StructWire
{
public:

    StructWireScalar(DataStream& aDs) : StructWire{aDs}
    { }

    // std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // uint32_t dbId;

    // Color wireColor;

    // int32_t startX;
    // int32_t startY;
    // int32_t endX;
    // int32_t endY;

    // LineWidth wireLineWidth;
    // LineStyle wireLineStyle;
};


#endif // STRUCTWIRESCALAR_HPP