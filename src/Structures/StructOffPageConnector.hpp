#ifndef STRUCTOFFPAGECONNECTOR_HPP
#define STRUCTOFFPAGECONNECTOR_HPP

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructGraphicInst.hpp"

namespace OOCP
{
class StructOffPageConnector : public StructGraphicInst
{
public:
    StructOffPageConnector(StreamContext& aCtx)
        : StructGraphicInst{aCtx}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    Structure getObjectType() const override
    {
        return Structure::OffPageConnector;
    }
};

[[maybe_unused]] static std::string to_string(const StructOffPageConnector& aObj)
{
    return aObj.to_string();
}

inline std::string StructOffPageConnector::to_string() const
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());

    str += StructGraphicInst::to_string();

    return str;
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StructOffPageConnector& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP

#endif // STRUCTOFFPAGECONNECTOR_HPP