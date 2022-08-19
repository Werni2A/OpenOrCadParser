#ifndef GENERAL_H
#define GENERAL_H


#include <cstdint>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <magic_enum.hpp>
#include <nameof.hpp>

#include "Exception.hpp"


#define TURN_MSG_STYLE_ON (false)


namespace fs = std::filesystem;


/**
 * @brief Version of the file format.
 * @warning This is not an official version but was introduced
 *          by myself as I don't know how or where the acutal
 *          version number is stored.
 */
enum class FileFormatVersion
{
    Unknown,
    A, // Oldest version
    B,
    C  // Latest Version
};


enum class FileType
{
    Library,
    Schematic
};


[[maybe_unused]]
static std::string getOpeningMsg(const char* aFuncName, size_t aCurrOffset)
{
    return fmt::format("0x{:08x}: Beginning {}", aCurrOffset, aFuncName);
}


[[maybe_unused]]
static std::string getClosingMsg(const char* aFuncName, size_t aCurrOffset)
{
    return fmt::format("0x{:08x}: Ending {}", aCurrOffset, aFuncName);
}


[[maybe_unused]]
static std::string indent(std::string str, size_t level)
{
    const std::string indent = "  ";
    const std::string delimiter = fmt::format("\n");

    std::vector<std::string> lines;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        lines.push_back(fmt::format("{}\n", std::move(token)));
        str.erase(0, pos + delimiter.length());
    }

    lines.push_back(std::move(str));

    std::string indentedStr;
    for(auto&& line : lines)
    {
        for(size_t i = 0u; i < level; ++i)
        {
            indentedStr += indent;
        }
        indentedStr += line;
    }

    return indentedStr;
}


[[maybe_unused]]
static std::string indent(size_t level)
{
    return indent("", level);
}


[[maybe_unused]] static uint8_t GetBit(size_t bitPos, uint32_t val)
{
    if(bitPos >= sizeof(val) * 8u)
    {
        throw std::invalid_argument("bitPos < " + std::to_string(sizeof(val) * 8u) + " must hold!");
    }

    return (val >> bitPos) & 0x01;
}


[[maybe_unused]]
static std::string DateTimeToStr(const time_t& unixts)
{
    return std::string(std::ctime(&unixts));
}


// @todo not implemented yet
[[maybe_unused]]
static std::string TimezoneToStr(int16_t timezone)
{
//   std::time_t rawtime;
//   std::tm* timeInfo;

//   const size_t lenBuffer = 24u;
//   char buffer[lenBuffer];

//   std::time(&rawtime);
//   timeInfo = std::localtime(&rawtime);

//   std::strftime(buffer, lenBuffer, "%z %Z", timeInfo);

  return std::to_string(timezone);
}


/**
 * @brief Convert fix point coordinate to floating point.
 * @note OrCAD stores all coorinates as fixed point value
 *       instead of the actual floating point representation.
 *
 * @param point Fix point coordiante.
 * @return double Floating point coordinate.
 */
[[maybe_unused]]
static double ToFP(int16_t point)
{
    return static_cast<double>(point) / 100.0;
}


template<typename TEnum, typename TVal>
static constexpr TEnum ToEnum(TVal aVal)
{
    const auto enumEntry = magic_enum::enum_cast<TEnum>(aVal);

    if(!enumEntry.has_value())
    {
        throw InvalidEnumEntry<TEnum, TVal>(aVal);
    }

    return enumEntry.value();
}


#endif // GENERAL_H