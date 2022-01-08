#ifndef DIRECTORYSTRUCT_H
#define DIRECTORYSTRUCT_H


#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../General.hpp"
#include "../Enums/ComponentType.hpp"


struct DirItemType
{
    std::string name;

    ComponentType componentType;

    int16_t timezone; // @todo Write to Kaitai file: Refer to http://time.unitarium.com/military/ for more details.
};


[[maybe_unused]]
static std::string to_string(const DirItemType& aObj)
{
    std::string str;

    str += std::string(nameof::nameof_type<decltype(aObj)>()) + ":" + newLine();
    str += indent(1) + "name  = " + aObj.name + newLine();
    str += indent(1) + "componentType = " + to_string(aObj.componentType) + newLine();
    str += indent(1) + "timezone      = " + TimezoneToStr(aObj.timezone)  + newLine();

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

    str += "DirectoryStruct:" + newLine();
    str += indent(1) + "lastModifiedDate = " + DateTimeToStr(aObj.lastModifiedDate) + newLine();;

    str += indent(1) + "items:" + newLine();
    for(size_t i = 0u; i < aObj.items.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(aObj.items[i]), 2);
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