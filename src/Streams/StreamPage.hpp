#ifndef STREAMPAGE_HPP
#define STREAMPAGE_HPP


#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"
#include "PageSettings.hpp"


class StreamPage : public CommonBase
{
public:

    StreamPage(DataStream& aDs) : CommonBase{aDs}, name{}, pageSize{}, pageSettings{mDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    std::string  name;
    std::string  pageSize;
    PageSettings pageSettings;
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