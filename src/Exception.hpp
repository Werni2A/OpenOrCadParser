#ifndef EXCEPTION_H
#define EXCEPTION_H


#include <stdexcept>
#include <string>


// @todo pass file format version of our current file and display it in the error message!
struct FileFormatChanged : public std::runtime_error
{
    FileFormatChanged(const char* aStructName) :
        std::runtime_error((std::string(aStructName) + " structure changed (differs between file format versions)!").c_str())
    { }
};


struct MisinterpretedData : public std::runtime_error
{
    MisinterpretedData(const char* aStructName, size_t aStartOffset, size_t aExpectedByteLen, size_t aCurrOffset) :
        std::runtime_error((std::string(aStructName) + " data size check failed. 0x" + ToHex(aStartOffset, 8)
                                 + " + 0x" + ToHex(aExpectedByteLen, 8) + " != 0x" + ToHex(aCurrOffset, 8)).c_str())
    { }
};


struct MissingFileFormatCheck : public std::runtime_error
{
    MissingFileFormatCheck(const char* aFunctionName, size_t aLine, FileFormatVersion aVersion) :
        std::runtime_error((std::string(aFunctionName) + " in line " + std::to_string(aLine) +
            " is missing a file format check for version " + std::to_string(static_cast<int>(aVersion))).c_str())
    { }
};


#endif // EXCEPTION_H