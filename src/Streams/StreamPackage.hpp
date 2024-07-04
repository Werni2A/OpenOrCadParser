#ifndef STREAMPACKAGE_HPP
#define STREAMPACKAGE_HPP

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Stream.hpp"
#include "Structures/StructDevice.hpp"
#include "Structures/StructLibraryPart.hpp"
#include "Structures/StructPackage.hpp"
#include "Structures/StructPartCell.hpp"

namespace OOCP
{
class StreamPackage : public Stream
{
public:
    StreamPackage(ContainerContext& aCtx, const fs::path& aInputStream)
        : Stream{aCtx, aInputStream},
          partCells{},
          libraryParts{},
          package{}
    {
    }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    virtual StreamType getStreamType() const override
    {
        return StreamType::Package;
    }

    std::vector<std::unique_ptr<StructPartCell>> partCells;
    std::vector<std::unique_ptr<StructLibraryPart>> libraryParts;

    std::unique_ptr<StructPackage> package;
};

[[maybe_unused]] static std::string to_string(const StreamPackage& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}partCells:\n", indent(1));
    for(size_t i = 0u; i < aObj.partCells.size(); ++i)
    {
        if(aObj.partCells[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.partCells[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}libraryParts:\n", indent(1));
    for(size_t i = 0u; i < aObj.libraryParts.size(); ++i)
    {
        if(aObj.libraryParts[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.libraryParts[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}package:\n", indent(1));
    if(aObj.package)
    {
        str += indent(aObj.package->to_string(), 2);
    }

    return str;
}

inline std::string StreamPackage::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StreamPackage& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}
} // namespace OOCP
#endif // STREAMPACKAGE_HPP