#ifndef PRIMSYMBOLVECTOR_HPP
#define PRIMSYMBOLVECTOR_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Primitives/PrimBase.hpp"

namespace OOCP
{
class PrimSymbolVector
    : public PrimBase // @todo probably derive from PrimBase, s.t. we can add itself to primitives list
{
public:
    PrimSymbolVector(StreamContext& aCtx)
        : PrimBase{aCtx},
          locX{0},
          locY{0},
          name{},
          primitives{}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Primitive getObjectType() const override
    {
        return Primitive::SymbolVector;
    }

    int16_t locX;
    int16_t locY;

    std::string name;

    // @note The positions of the primitives are
    //       relative to the offset provided by
    //       locX and locY.
    std::vector<PrimBase*> primitives;
};

[[maybe_unused]] static std::string to_string(const PrimSymbolVector& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}locX = {}\n", indent(1), aObj.locX);
    str += fmt::format("{}locY = {}\n", indent(1), aObj.locY);
    str += fmt::format("{}name = {}\n", indent(1), aObj.name);

    str += fmt::format("{}primitives:\n", indent(1));
    for(size_t i = 0u; i < aObj.primitives.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.primitives[i]->to_string()), 2);
    }

    return str;
}

inline std::string PrimSymbolVector::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const PrimSymbolVector& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP

#endif // PRIMSYMBOLVECTOR_HPP