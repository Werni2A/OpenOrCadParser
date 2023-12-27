#ifndef GENERAL_HPP
#define GENERAL_HPP


#include <cstdint>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <magic_enum.hpp>
#include <nameof.hpp>

#include "Exception.hpp"


namespace fs = std::filesystem;


enum class FileType;
enum class FileFormatVersion;


/**
 * @brief Version of the file format.
 * @warning This is not an official version but was introduced
 *          by myself as I don't know how or where the actual
 *          version number is stored.
 */
enum class FileFormatVersion
{
    A = 0, // Oldest version
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P, // Latest Version
    Unknown,
};


enum class FileType
{
    Library,
    Schematic
};


// Copied from StackExchange [1] and slightly modified
// to throw on invalid cast and pass nullptrs through
// [1] https://codereview.stackexchange.com/a/280784
template<typename T, typename S>
static std::unique_ptr<T> dynamic_pointer_cast(std::unique_ptr<S>&& p)
{
    // Do not try to cast nullptr
    if(!p)
    {
        return {};
    }

    if (T* const converted = dynamic_cast<T*>(p.get())) {
        // cast succeeded; clear input
        p.release();
        return std::unique_ptr<T>{converted};
    }

    throw std::runtime_error{fmt::format("{} is not of type {}",
        NAMEOF_FULL_TYPE_RTTI(*p.get()),
        nameof::nameof_type<T>())};
}


template<typename T>
static std::string getMethodName(const T* aClass, const char* aFuncName)
{
    return fmt::format("{}::{}", NAMEOF_TYPE_RTTI(*aClass), aFuncName);
}


[[maybe_unused]]
static std::string getOpeningMsg(const std::string& aClassFuncName, size_t aCurrOffset)
{
    return fmt::format("0x{:08x}: Beginning {}", aCurrOffset, aClassFuncName);
}


[[maybe_unused]]
static std::string getClosingMsg(const std::string& aClassFuncName, size_t aCurrOffset)
{
    return fmt::format("0x{:08x}: Ending {}", aCurrOffset, aClassFuncName);
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


#endif // GENERAL_HPP