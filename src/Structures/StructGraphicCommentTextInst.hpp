#ifndef STRUCTGRAPHICCOMMENTTEXTINST_HPP
#define STRUCTGRAPHICCOMMENTTEXTINST_HPP


#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


class StructGraphicCommentTextInst : public CommonBase
{
public:

    StructGraphicCommentTextInst(DataStream& aDs) : CommonBase{aDs}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

};


[[maybe_unused]]
static std::string to_string(const StructGraphicCommentTextInst& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructGraphicCommentTextInst::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicCommentTextInst& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STRUCTGRAPHICCOMMENTTEXTINST_HPP