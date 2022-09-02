#ifndef STREAMDSNSTREAM_HPP
#define STREAMDSNSTREAM_HPP


#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"


struct StreamDsnStream
{

};


[[maybe_unused]]
static std::string to_string(const StreamDsnStream& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());

    return str;
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const StreamDsnStream& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // STREAMDSNSTREAM_HPP