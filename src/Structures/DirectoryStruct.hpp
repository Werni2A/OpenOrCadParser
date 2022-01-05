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
static std::string to_string(const DirItemType& dirItemType)
{
    std::string str;

    str += "DirItemType:" + newLine();
    str += indent(1) + "name  = " + dirItemType.name + newLine();
    str += indent(1) + "componentType = " + to_string(dirItemType.componentType) + newLine();
    str += indent(1) + "timezone      = " + TimezoneToStr(dirItemType.timezone)  + newLine();

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const DirItemType& dirItemType)
{
    os << to_string(dirItemType);

    return os;
}


struct DirectoryStruct
{
    time_t lastModifiedDate;

    std::vector<DirItemType> items;
};


[[maybe_unused]]
static std::string to_string(const DirectoryStruct& directoryStruct)
{
    std::string str;

    str += "DirectoryStruct:" + newLine();
    str += indent(1) + "lastModifiedDate = " + DateTimeToStr(directoryStruct.lastModifiedDate) + newLine();;

    str += indent(1) + "items:" + newLine();
    for(size_t i = 0u; i < directoryStruct.items.size(); ++i)
    {
        str += indent(std::to_string(i) + ": " + to_string(directoryStruct.items[i]), 2);
    }

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& os, const DirectoryStruct& directoryStruct)
{
    os << to_string(directoryStruct);
    return os;
}


#endif // DIRECTORYSTRUCT_H