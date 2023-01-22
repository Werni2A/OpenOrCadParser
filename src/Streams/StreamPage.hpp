#ifndef STREAMPAGE_HPP
#define STREAMPAGE_HPP


#include <memory>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"
#include "PageSettings.hpp"
#include "Structures/StructPartInst.hpp"
#include "Structures/StructPort.hpp"
#include "Structures/StructT0x34.hpp"
#include "Structures/StructT0x35.hpp"
#include "Structures/StructWire.hpp"


class StreamPage : public CommonBase
{
public:

    StreamPage(DataStream& aDs) : CommonBase{aDs}, name{}, pageSize{}, pageSettings{mDs},
        t0x34s{}, t0x35s{}, wires{}, partInsts{}, ports{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string  name;
    std::string  pageSize;
    PageSettings pageSettings;

    std::vector<std::unique_ptr<StructT0x34>>    t0x34s;
    std::vector<std::unique_ptr<StructT0x35>>    t0x35s;
    std::vector<std::unique_ptr<StructWire>>     wires;
    std::vector<std::unique_ptr<StructPartInst>> partInsts;
    std::vector<std::unique_ptr<StructPort>>     ports;
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
        str += indent(fmt::format("[{}]: {}", i, aObj.t0x34s[i]->to_string()), 2);
    }

    str += fmt::format("{}t0x35s:\n", indent(1));
    for(size_t i = 0u; i < aObj.t0x35s.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.t0x35s[i]->to_string()), 2);
    }

    str += fmt::format("{}wires:\n", indent(1));
    for(size_t i = 0u; i < aObj.wires.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.wires[i]->to_string()), 2);
    }

    str += fmt::format("{}partInsts:\n", indent(1));
    for(size_t i = 0u; i < aObj.partInsts.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.partInsts[i]->to_string()), 2);
    }

    str += fmt::format("{}ports:\n", indent(1));
    for(size_t i = 0u; i < aObj.ports.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.ports[i]->to_string()), 2);
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