#ifndef EXCEPTION_H
#define EXCEPTION_H


#include <stdexcept>
#include <string>

#include <magic_enum.hpp>
#include <nameof.hpp>

#include "General.hpp"


// Forward declaration
template<typename T>
[[maybe_unused]] static std::string ToHex(T val, size_t digits);
enum class FileFormatVersion;


template<typename T, typename TVal>
struct InvalidEnumEntry : public std::invalid_argument
{
    InvalidEnumEntry(TVal aVal) :
        std::invalid_argument("Enum `" + std::string(nameof::nameof_type<T>())
            + "` does not implement value " + std::to_string(aVal) + "!")
    { }
};


// @todo pass file format version of our current file and display it in the error message!
struct FileFormatChanged : public std::runtime_error
{
    FileFormatChanged(std::string aStructName) :
        std::runtime_error((aStructName + " structure changed (differs between file format versions)!").c_str())
    { }
};


struct MisinterpretedData : public std::runtime_error
{
    MisinterpretedData(std::string aStructName, size_t aStartOffset, size_t aExpectedByteLen, size_t aCurrOffset) :
        std::runtime_error((aStructName + " data size check failed. 0x" + ToHex(aStartOffset, 8)
                                 + " + 0x" + ToHex(aExpectedByteLen, 8) + " != 0x" + ToHex(aCurrOffset, 8)).c_str())
    { }
};


struct MissingFileFormatCheck : public std::runtime_error
{
    MissingFileFormatCheck(std::string aFunctionName, size_t aLine, FileFormatVersion aVersion) :
        std::runtime_error((aFunctionName + " in line " + std::to_string(aLine) +
            " is missing a file format check for version " + std::to_string(static_cast<int>(aVersion))).c_str())
    { }
};


#endif // EXCEPTION_H