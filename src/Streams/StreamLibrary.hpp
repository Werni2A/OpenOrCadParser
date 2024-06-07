#ifndef STREAMSYMBOLSLIBRARY_HPP
#define STREAMSYMBOLSLIBRARY_HPP


#include <cstdint>
#include <ctime>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <nameof.hpp>

#include "PageSettings.hpp"
#include "Stream.hpp"
#include "Win32/LOGFONTA.hpp"


class StreamLibrary : public Stream
{
public:

    StreamLibrary(ContainerContext& aCtx, const fs::path& aInputStream) : Stream{aCtx, aInputStream}, introduction{}, createDate{0}, modifyDate{0},
        textFonts{}, strLstPartField{}, pageSettings{mCtx}, strLst{}, partAliases{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    // void accept(Visitor& aVisitor) const override
    // {
    //     aVisitor.visit(*this);
    // }

    virtual StreamType getStreamType() const override
    {
        return StreamType::Library;
    }

    std::string introduction;

    time_t createDate;
    time_t modifyDate;

    std::vector<LOGFONTA> textFonts;

    std::vector<std::string> strLstPartField;

    PageSettings pageSettings;

    std::vector<std::string> strLst;

    // See OrCAD: 'Package Properties' -> 'Part Aliases'
    std::vector<std::pair<std::string, std::string>> partAliases; //!< .first = Alias, .second = Package
};


[[maybe_unused]]
static std::string to_string(const StreamLibrary& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}introduction = {}\n", indent(1), aObj.introduction);
    str += fmt::format("{}createDate   = {}\n", indent(1), DateTimeToStr(aObj.createDate));
    str += fmt::format("{}modifyDate   = {}\n", indent(1), DateTimeToStr(aObj.modifyDate));

    str += fmt::format("{}textFonts:\n", indent(1));
    for(size_t i = 0u; i < aObj.textFonts.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}", i, aObj.textFonts[i].to_string()), 2);
    }

    str += fmt::format("{}strLstPartField:\n", indent(1));
    for(size_t i = 0u; i < aObj.strLstPartField.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}\n", i, aObj.strLstPartField[i]), 2);
    }

    str += fmt::format("{}pageSettings:\n", indent(1));
    str += indent(aObj.pageSettings.to_string(), 2);

    str += fmt::format("{}strLst:\n", indent(1));
    for(size_t i = 0u; i < aObj.strLst.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {}\n", i, aObj.strLst[i]), 2);
    }

    str += fmt::format("{}partAliases:\n", indent(1));
    for(size_t i = 0u; i < aObj.partAliases.size(); ++i)
    {
        str += indent(fmt::format("[{}]: {} = {}\n", i, aObj.partAliases[i].first, aObj.partAliases[i].second), 2);
    }

    return str;
}


inline std::string StreamLibrary::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamLibrary& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}


#endif // STREAMSYMBOLSLIBRARY_HPP