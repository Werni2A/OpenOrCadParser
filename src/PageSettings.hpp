#ifndef PAGESETTINGS_HPP
#define PAGESETTINGS_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Record.hpp"


class PageSettings : public Other
{
public:

    PageSettings(StreamContext& aCtx) : Other{aCtx},
        createDateTime{0}, modifyDateTime{0},
        width{0}, height{0},
        pinToPin{0},
        horizontalCount{0}, verticalCount{0},
        horizontalWidth{0}, verticalWidth{0},
        horizontalChar{0}, horizontalAscending{0},
        verticalChar{0}, verticalAscending{0},
        isMetric{0},
        borderDisplayed{0}, borderPrinted{0},
        gridRefDisplayed{0}, gridRefPrinted{0},
        titleblockDisplayed{0}, titleblockPrinted{0},
        ansiGridRefs{0}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    time_t createDateTime;
    time_t modifyDateTime;

    uint32_t width;
    uint32_t height;

    uint32_t pinToPin; //!< Pin-to-pin spacing

    uint16_t horizontalCount; //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint16_t verticalCount;   //!< See 'Schematic Page Properties' -> 'Grid Reference'

    uint32_t horizontalWidth; //!< See 'Schematic Page Properties' -> 'Grid Reference'
    uint32_t verticalWidth;   //!< See 'Schematic Page Properties' -> 'Grid Reference'

    // See 'Schematic Page Properties' -> 'Grid Reference' -> 'Horizontal' ->
    // 'Alphabetic' = 1
    // 'Numeric'    = 0
    uint32_t horizontalChar;

    // See 'Schematic Page Properties' -> 'Grid Reference' -> 'Horizontal'
    // Select 'ascending' (1) or 'decending' (0).
    uint32_t horizontalAscending;

    // See 'Schematic Page Properties' -> 'Grid Reference' -> 'Vertical' ->
    // 'Alphabetic' = 1
    // 'Numeric'    = 0
    uint32_t verticalChar;

    // See 'Schematic Page Properties' -> 'Grid Reference' -> 'Vertical'
    // Select 'ascending' (1) or 'decending' (0).
    uint32_t verticalAscending;

    uint32_t isMetric;
    uint32_t borderDisplayed;
    uint32_t borderPrinted;
    uint32_t gridRefDisplayed;
    uint32_t gridRefPrinted;
    uint32_t titleblockDisplayed;
    uint32_t titleblockPrinted;

    // Use ANSI grid references.
    // See 'Schematic Page Properties' -> 'Grid Reference'
    uint32_t ansiGridRefs;
};


[[maybe_unused]]
static std::string to_string(const PageSettings& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}createDateTime      = {}\n", indent(1), DateTimeToStr(aObj.createDateTime));
    str += fmt::format("{}modifyDateTime      = {}\n", indent(1), DateTimeToStr(aObj.modifyDateTime));
    str += fmt::format("{}width               = {}\n", indent(1), aObj. width);
    str += fmt::format("{}height              = {}\n", indent(1), aObj.height);
    str += fmt::format("{}pinToPin            = {}\n", indent(1), aObj.pinToPin);
    str += fmt::format("{}horizontalCount     = {}\n", indent(1), aObj.horizontalCount);
    str += fmt::format("{}verticalCount       = {}\n", indent(1), aObj.verticalCount);
    str += fmt::format("{}horizontalWidth     = {}\n", indent(1), aObj.horizontalWidth);
    str += fmt::format("{}verticalWidth       = {}\n", indent(1), aObj.verticalWidth);
    str += fmt::format("{}horizontalChar      = {}\n", indent(1), aObj.horizontalChar);
    str += fmt::format("{}horizontalAscending = {}\n", indent(1), aObj.horizontalAscending);
    str += fmt::format("{}verticalChar        = {}\n", indent(1), aObj.verticalChar);
    str += fmt::format("{}verticalAscending   = {}\n", indent(1), aObj.verticalAscending);
    str += fmt::format("{}isMetric            = {}\n", indent(1), aObj.isMetric);
    str += fmt::format("{}borderDisplayed     = {}\n", indent(1), aObj.borderDisplayed);
    str += fmt::format("{}borderPrinted       = {}\n", indent(1), aObj.borderPrinted);
    str += fmt::format("{}gridRefDisplayed    = {}\n", indent(1), aObj.gridRefDisplayed);
    str += fmt::format("{}gridRefPrinted      = {}\n", indent(1), aObj.gridRefPrinted);
    str += fmt::format("{}titleblockDisplayed = {}\n", indent(1), aObj.titleblockDisplayed);
    str += fmt::format("{}titleblockPrinted   = {}\n", indent(1), aObj.titleblockPrinted);
    str += fmt::format("{}ansiGridRefs        = {}\n", indent(1), aObj.ansiGridRefs);

    return str;
}


inline std::string PageSettings::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PageSettings& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // PAGESETTINGS_HPP