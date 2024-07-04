#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>
#include <string>

#include <fmt/core.h>
#include <magic_enum.hpp>
#include <nameof.hpp>

#include "General.hpp"

// Forward declaration
enum class FileFormatVersion;

template <typename T, typename TVal> struct InvalidEnumEntry : public std::invalid_argument
{
    InvalidEnumEntry(TVal aVal)
        : std::invalid_argument(fmt::format("Enum `{}` does not implement value {}!", nameof::nameof_type<T>(), aVal))
    {
    }
};

// @todo pass file format version of our current file and display it in the error message!
struct FileFormatChanged : public std::runtime_error
{
    FileFormatChanged(const std::string& aStructName)
        : std::runtime_error((aStructName + " structure changed (differs between file format versions)!"))
    {
    }
};

struct MisinterpretedData : public std::runtime_error
{
    MisinterpretedData(const std::string& aStructName, size_t aStartOffset, size_t aExpectedByteLen, size_t aCurrOffset)
        : std::runtime_error(fmt::format("{} data size check failed. 0x{:08x} (start offset) + 0x{:08x} (expected Byte "
                                         "length) != 0x{:08x} (current offset)",
              aStructName, aStartOffset, aExpectedByteLen, aCurrOffset))
    {
    }
};

struct MissingFileFormatCheck : public std::runtime_error
{
    MissingFileFormatCheck(const std::string& aFunctionName, size_t aLine, FileFormatVersion aVersion)
        : std::runtime_error(fmt::format("{} in line {} is missing a file format check for version {}", aFunctionName,
              aLine, static_cast<int>(aVersion)))
    {
    }
};

#endif // EXCEPTION_HPP