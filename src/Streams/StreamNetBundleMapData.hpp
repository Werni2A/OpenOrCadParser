#ifndef STREAMNETBUNDLEMAPDATA_HPP
#define STREAMNETBUNDLEMAPDATA_HPP


#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StreamNetBundleMapData : public CommonBase
{
public:

    StreamNetBundleMapData(DataStream& aDs) : CommonBase{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StreamNetBundleMapData& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StreamNetBundleMapData::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamNetBundleMapData& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMNETBUNDLEMAPDATA_HPP