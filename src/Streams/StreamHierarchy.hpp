#ifndef STREAMHIERARCHY_HPP
#define STREAMHIERARCHY_HPP

#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Stream.hpp"
#include "Structures/StructNetDbIdMapping.hpp"
#include "Structures/StructSomeHierarchyBase.hpp"
#include "Structures/StructSthInHierarchy1.hpp"
#include "Structures/StructSthInHierarchy2.hpp"
#include "Structures/StructSthInHierarchy3.hpp"
#include "Structures/StructT0x5b.hpp"

namespace OOCP
{
class StreamHierarchy : public Stream
{
public:
    StreamHierarchy(ContainerContext& aCtx, const fs::path& aInputStream)
        : Stream{aCtx, aInputStream},
          netDbIdMappings{},
          sthInHierarchy3s{},
          t0x5bs{},
          sthInHierarchy1s{},
          someHierarchyBases{}
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
        return StreamType::Hierarchy;
    }

    std::vector<std::unique_ptr<StructSthInHierarchy2>> sthInHierarchy2s;
    std::vector<std::unique_ptr<StructNetDbIdMapping>> netDbIdMappings;
    std::vector<std::unique_ptr<StructSthInHierarchy3>> sthInHierarchy3s;
    std::vector<std::unique_ptr<StructT0x5b>> t0x5bs;
    std::vector<std::unique_ptr<StructSthInHierarchy1>> sthInHierarchy1s;
    std::vector<std::unique_ptr<StructSomeHierarchyBase>> someHierarchyBases;
};

[[maybe_unused]]
static std::string to_string(const StreamHierarchy& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}sthInHierarchy2s:\n", indent(1));
    for(size_t i = 0u; i < aObj.sthInHierarchy2s.size(); ++i)
    {
        if(aObj.sthInHierarchy2s[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.sthInHierarchy2s[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}netDbIdMappings:\n", indent(1));
    for(size_t i = 0u; i < aObj.netDbIdMappings.size(); ++i)
    {
        if(aObj.netDbIdMappings[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.netDbIdMappings[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}sthInHierarchy3s:\n", indent(1));
    for(size_t i = 0u; i < aObj.sthInHierarchy3s.size(); ++i)
    {
        if(aObj.sthInHierarchy3s[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.sthInHierarchy3s[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}t0x5bs:\n", indent(1));
    for(size_t i = 0u; i < aObj.t0x5bs.size(); ++i)
    {
        if(aObj.t0x5bs[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.t0x5bs[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}sthInHierarchy1s:\n", indent(1));
    for(size_t i = 0u; i < aObj.sthInHierarchy1s.size(); ++i)
    {
        if(aObj.sthInHierarchy1s[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.sthInHierarchy1s[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}someHierarchyBases:\n", indent(1));
    for(size_t i = 0u; i < aObj.someHierarchyBases.size(); ++i)
    {
        if(aObj.someHierarchyBases[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.someHierarchyBases[i]->to_string()), 2);
        }
    }

    return str;
}

inline std::string StreamHierarchy::to_string() const
{
    return OOCP::to_string(*this);
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamHierarchy& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}
} // namespace OOCP

#endif // STREAMHIERARCHY_HPP