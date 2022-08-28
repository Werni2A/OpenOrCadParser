#ifndef DSNSTREAM_H
#define DSNSTREAM_H


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "../General.hpp"


struct DsnStream
{

};


[[maybe_unused]]
static std::string to_string(const DsnStream& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const DsnStream& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // DSNSTREAM_H