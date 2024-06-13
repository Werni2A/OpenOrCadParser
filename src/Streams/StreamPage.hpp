#ifndef STREAMPAGE_HPP
#define STREAMPAGE_HPP


#include <memory>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "PageSettings.hpp"
#include "Stream.hpp"
#include "Structures/StructBusEntry.hpp"
#include "Structures/StructERCObject.hpp"
#include "Structures/StructGlobal.hpp"
#include "Structures/StructGraphicInst.hpp"
#include "Structures/StructOffPageConnector.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructPort.hpp"
#include "Structures/StructT0x34.hpp"
#include "Structures/StructT0x35.hpp"
#include "Structures/StructTitleBlock.hpp"
#include "Structures/StructWire.hpp"


class StreamPage : public Stream
{
public:

    StreamPage(ContainerContext& aCtx, const fs::path& aInputStream) : Stream{aCtx, aInputStream}, name{}, pageSize{}, pageSettings{mCtx},
        titleBlocks{}, t0x34s{}, t0x35s{}, wires{}, partInsts{}, ports{}, globals{}, offPageConnectors{},
        ercObjects{}, busEntries{}, graphicInsts{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    virtual StreamType getStreamType() const override
    {
        return StreamType::Page;
    }

    std::string  name;
    std::string  pageSize;
    PageSettings pageSettings;

    std::vector<std::unique_ptr<StructTitleBlock>>       titleBlocks;
    std::vector<std::unique_ptr<StructT0x34>>            t0x34s;
    std::vector<std::unique_ptr<StructT0x35>>            t0x35s;
    std::vector<std::unique_ptr<StructWire>>             wires;
    std::vector<std::unique_ptr<StructPartInst>>         partInsts;
    std::vector<std::unique_ptr<StructPort>>             ports;
    std::vector<std::unique_ptr<StructGlobal>>           globals;
    std::vector<std::unique_ptr<StructOffPageConnector>> offPageConnectors;
    std::vector<std::unique_ptr<StructERCObject>>        ercObjects;
    std::vector<std::unique_ptr<StructBusEntry>>         busEntries;
    std::vector<std::unique_ptr<StructGraphicInst>>      graphicInsts;
};


[[maybe_unused]]
static std::string to_string(const StreamPage& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name     = {}\n", indent(1), aObj.name);
    str += fmt::format("{}pageSize = {}\n", indent(1), aObj.pageSize);
    str += fmt::format("{}pageSettings:\n", indent(1));
    str += indent(aObj.pageSettings.to_string(), 2);

    str += fmt::format("{}t0x34s:\n", indent(1));
    for(size_t i = 0u; i < aObj.t0x34s.size(); ++i)
    {
        if(aObj.t0x34s[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.t0x34s[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}titleBlocks:\n", indent(1));
    for(size_t i = 0u; i < aObj.titleBlocks.size(); ++i)
    {
        if(aObj.titleBlocks[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.titleBlocks[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}t0x35s:\n", indent(1));
    for(size_t i = 0u; i < aObj.t0x35s.size(); ++i)
    {
        if(aObj.t0x35s[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.t0x35s[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}wires:\n", indent(1));
    for(size_t i = 0u; i < aObj.wires.size(); ++i)
    {
        if(aObj.wires[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.wires[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}partInsts:\n", indent(1));
    for(size_t i = 0u; i < aObj.partInsts.size(); ++i)
    {
        if(aObj.partInsts[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.partInsts[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}ports:\n", indent(1));
    for(size_t i = 0u; i < aObj.ports.size(); ++i)
    {
        if(aObj.ports[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.ports[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}globals:\n", indent(1));
    for(size_t i = 0u; i < aObj.globals.size(); ++i)
    {
        if(aObj.globals[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.globals[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}offPageConnectors:\n", indent(1));
    for(size_t i = 0u; i < aObj.offPageConnectors.size(); ++i)
    {
        if(aObj.offPageConnectors[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.offPageConnectors[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}ercObjects:\n", indent(1));
    for(size_t i = 0u; i < aObj.ercObjects.size(); ++i)
    {
        if(aObj.ercObjects[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.ercObjects[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}busEntries:\n", indent(1));
    for(size_t i = 0u; i < aObj.busEntries.size(); ++i)
    {
        if(aObj.busEntries[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.busEntries[i]->to_string()), 2);
        }
    }

    str += fmt::format("{}graphicInsts:\n", indent(1));
    for(size_t i = 0u; i < aObj.graphicInsts.size(); ++i)
    {
        if(aObj.graphicInsts[i])
        {
            str += indent(fmt::format("[{}]: {}", i, aObj.graphicInsts[i]->to_string()), 2);
        }
    }

    return str;
}


inline std::string StreamPage::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamPage& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMPAGE_HPP