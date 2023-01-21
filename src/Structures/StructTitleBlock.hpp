#ifndef STRUCTTITLEBLOCK_HPP
#define STRUCTTITLEBLOCK_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StructTitleBlock : public CommonBase
{
public:

    StructTitleBlock(DataStream& aDs) : CommonBase{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructTitleBlock& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructTitleBlock::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructTitleBlock& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTTITLEBLOCK_HPP