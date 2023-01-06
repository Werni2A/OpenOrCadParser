#ifndef STREAMSYMBOL_HPP
#define STREAMSYMBOL_HPP


#include <memory>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StreamSymbol : public CommonBase
{
public:

    StreamSymbol(DataStream& aDs) : CommonBase{aDs} , structures{}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;


    std::vector<std::unique_ptr<CommonBase>> structures;
};


[[maybe_unused]]
static std::string to_string(const StreamSymbol& aObj)
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


inline std::string StreamSymbol::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamSymbol& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMSYMBOL_HPP