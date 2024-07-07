#ifndef STRUCTGRAPHICINST_HPP
#define STRUCTGRAPHICINST_HPP

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/Color.hpp"
#include "General.hpp"
#include "Record.hpp"
#include "Structures/StructSthInPages0.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"

namespace OOCP
{
/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructGraphicInst : public Record
{
public:
    StructGraphicInst(StreamContext& aCtx)
        : Record{aCtx},
          name{},
          dbId{0},
          locX{0},
          locY{0},
          x1{0},
          y1{0},
          x2{0},
          y2{0},
          color{Color::Default},
          symbolDisplayProps{},
          sthInPages0{}
    {
    }

    std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;
    void read(FutureDataLst& mLocalFutureLst, FileFormatVersion aVersion = FileFormatVersion::Unknown);

    Structure getObjectType() const override
    {
        return Structure::GraphicArcInst;
    }

    std::string name;

    uint32_t dbId;

    int16_t locX;
    int16_t locY;

    int16_t x1;
    int16_t y1;

    int16_t x2;
    int16_t y2;

    Color color;

    std::vector<std::unique_ptr<StructSymbolDisplayProp>> symbolDisplayProps;

    std::unique_ptr<StructSthInPages0> sthInPages0;
};

[[maybe_unused]]
static std::string to_string(const StructGraphicInst& aObj)
{
    return aObj.to_string();
}

inline std::string StructGraphicInst::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());
    str += fmt::format("{}name  = {}\n", indent(1), name);
    str += fmt::format("{}dbId  = {}\n", indent(1), dbId);
    str += fmt::format("{}locX  = {}\n", indent(1), locX);
    str += fmt::format("{}locY  = {}\n", indent(1), locY);
    str += fmt::format("{}x1    = {}\n", indent(1), x1);
    str += fmt::format("{}y1    = {}\n", indent(1), y1);
    str += fmt::format("{}x2    = {}\n", indent(1), x2);
    str += fmt::format("{}y2    = {}\n", indent(1), y2);
    str += fmt::format("{}color = {}\n", indent(1), OOCP::to_string(color));

    str += fmt::format("{}symbolDisplayProps:\n", indent(1));
    for(size_t i = 0u; i < symbolDisplayProps.size(); ++i)
    {
        if(symbolDisplayProps[i])
        {
            str += indent(fmt::format("[{}]: {}", i, symbolDisplayProps[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}sthInPages0:\n", indent(1));
    if(sthInPages0)
    {
        str += indent(sthInPages0->to_string(), 2);
    }

    return str;
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicInst& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTGRAPHICINST_HPP