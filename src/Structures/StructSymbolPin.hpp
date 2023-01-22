#ifndef STRUCTSYMBOLPIN_HPP
#define STRUCTSYMBOLPIN_HPP


#include <cstdint>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "CommonBase.hpp"
#include "Enums/PortType.hpp"
#include "PinShape.hpp"


/*!
 * @brief Pseudo structure that does not exist itself.
          It's only provided to group structures that
          are derived from it and extract common code
          into it.
 */
class StructSymbolPin : public CommonBase
{
public:

    StructSymbolPin(DataStream& aDs) : CommonBase{aDs}
    { }

    // std::string to_string() const override;

    // void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;
};


#endif // STRUCTSYMBOLPIN_HPP