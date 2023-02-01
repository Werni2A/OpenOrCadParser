#ifndef STRUCTWIRE_HPP
#define STRUCTWIRE_HPP


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
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructAlias.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"

/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructWire : public CommonBase
{
public:

    StructWire(ParserContext& aCtx) : CommonBase{aCtx}, id{0}, color{Color::Default},
        startX{0}, startY{0}, endX{0}, endY{0},
        lineWidth{LineWidth::Default}, lineStyle{LineStyle::Default}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    uint32_t  id;
    Color     color;

    int32_t   startX;
    int32_t   startY;
    int32_t   endX;
    int32_t   endY;

    std::vector<std::unique_ptr<StructAlias>>             aliases;
    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;

    LineWidth lineWidth;
    LineStyle lineStyle;
};


[[maybe_unused]]
static std::string to_string(const StructWire& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}id        = {}\n", indent(1), aObj.id);
    str += fmt::format("{}color     = {}\n", indent(1), to_string(aObj.color));
    str += fmt::format("{}startX    = {}\n", indent(1), aObj.startX);
    str += fmt::format("{}startY    = {}\n", indent(1), aObj.startY);
    str += fmt::format("{}endX      = {}\n", indent(1), aObj.endX);
    str += fmt::format("{}endY      = {}\n", indent(1), aObj.endY);

    str += fmt::format("{}aliases:\n", indent(1));
    for(size_t i = 0u; i < aObj.aliases.size(); ++i)
    {
        if(aObj.aliases[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.aliases[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < aObj.symbolDisplayProps.size(); ++i)
    {
        if(aObj.symbolDisplayProps[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.symbolDisplayProps[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}lineWidth = {}\n", indent(1), to_string(aObj.lineWidth));
    str += fmt::format("{}lineStyle = {}\n", indent(1), to_string(aObj.lineStyle));

    return str;
}


inline std::string StructWire::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructWire& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTWIRE_HPP