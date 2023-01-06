#ifndef STREAMPACKAGE_HPP
#define STREAMPACKAGE_HPP


#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"


class StreamPackage : public CommonBase
{
public:

    StreamPackage(DataStream& aDs) : CommonBase{aDs}, structures{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    FileFormatVersion predictVersion();

    std::vector<std::unique_ptr<CommonBase>> structures;
};


[[maybe_unused]]
static std::string to_string(const StreamPackage& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    str += fmt::format("{}structures:\n", indent(1));
    for(size_t i = 0u; i < aObj.structures.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.structures[i]->to_string()), 2);
    }

    return str;
}


inline std::string StreamPackage::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamPackage& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMPACKAGE_HPP