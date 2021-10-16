#ifndef GENERAL_H
#define GENERAL_H


#include <cstdint>
#include <iostream>
#include <string>


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


template<typename T>
[[maybe_unused]] static std::string ToHex(T val, size_t digits)
{
    // @todo Use fmt lib!
    char hex[16];
    std::string format = "%0" + std::to_string(digits) + "x";
    std::sprintf(hex, format.c_str(), static_cast<unsigned int>(val));
    return std::string(hex);
}


#endif // GENERAL_H