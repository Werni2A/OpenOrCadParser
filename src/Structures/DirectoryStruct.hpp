#ifndef DIRECTORYSTRUCT_H
#define DIRECTORYSTRUCT_H


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"
#include "../Enums/ComponentType.hpp"


struct DirItemType
{
    std::string name;

    ComponentType componentType;

    uint16_t fileFormatVersion; //!< @todo Verify this. It's likely but I'm not sure

    int16_t timezone; //!< Refer to http://time.unitarium.com/military/ for more details.
};


[[maybe_unused]]
static std::string to_string(const DirItemType& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}name          = {}\n", indent(1), aObj.name);
    str += fmt::format("{}componentType = {}\n", indent(1), to_string(aObj.componentType));
    str += fmt::format("{}timezone      = {}\n", indent(1), TimezoneToStr(aObj.timezone));

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const DirItemType& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


struct DirectoryStruct
{
    time_t lastModifiedDate;

    std::vector<DirItemType> items;
};


[[maybe_unused]]
static std::string to_string(const DirectoryStruct& aObj)
{
    std::string str;

    str += fmt::format("DirectoryStruct:\n");
    str += fmt::format("{}lastModifiedDate = {}\n", indent(1), DateTimeToStr(aObj.lastModifiedDate));

    str += fmt::format("{}items:\n", indent(1));
    for(size_t i = 0u; i < aObj.items.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.items[i])), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const DirectoryStruct& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // DIRECTORYSTRUCT_H