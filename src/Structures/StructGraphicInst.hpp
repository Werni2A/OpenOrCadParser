#ifndef STRUCTGRAPHICINST_HPP
#define STRUCTGRAPHICINST_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "General.hpp"


/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructGraphicInst : public CommonBase
{
public:

    StructGraphicInst(DataStream& aDs) : CommonBase{aDs}
    { }

    std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;
};


[[maybe_unused]]
static std::string to_string(const StructGraphicInst& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


inline std::string StructGraphicInst::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StructGraphicInst& aObj)
{
    aOs << to_string(aObj);

    return aOs;
}


#endif // STRUCTGRAPHICINST_HPP