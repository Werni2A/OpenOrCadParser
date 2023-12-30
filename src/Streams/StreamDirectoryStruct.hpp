#ifndef STREAMDIRECTORYSTRUCT_HPP
#define STREAMDIRECTORYSTRUCT_HPP


#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "Enums/ComponentType.hpp"
#include "General.hpp"
#include "Stream.hpp"


class DirItemType
{
public:

    DirItemType() : name{}, componentType{ComponentType::Graphic},
        fileFormatVersion{0}, timezone{0}
    { }

    std::string to_string() const;

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


inline std::string DirItemType::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const DirItemType& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


// Used to parse
// - `Cells Directory`
// - `ExportBlocks Directory`
// - `Graphics Directory`
// - `Packages Directory`
// - `Parts Directory`
// - `Symbols Directory`
// - `Views Directory`
class StreamDirectoryStruct : public Stream
{
public:

    StreamDirectoryStruct(ContainerContext& aCtx, const fs::path& aInputStream) : Stream{aCtx, aInputStream},
        lastModifiedDate{0}, items{}
    { }

    std::string to_string() const;

    void read(FileFormatVersion /* aVersion */ = FileFormatVersion::Unknown);

    void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    time_t lastModifiedDate;

    std::vector<DirItemType> items;
};


[[maybe_unused]]
static std::string to_string(const StreamDirectoryStruct& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}lastModifiedDate = {}\n", indent(1), DateTimeToStr(aObj.lastModifiedDate));

    str += fmt::format("{}items:\n", indent(1));
    for(size_t i = 0u; i < aObj.items.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, to_string(aObj.items[i])), 2);
    }

    return str;
}


inline std::string StreamDirectoryStruct::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamDirectoryStruct& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // STREAMDIRECTORYSTRUCT_HPP