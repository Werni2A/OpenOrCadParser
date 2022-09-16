#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "DataStream.hpp"
#include "General.hpp"


void DataStream::discardBytes(size_t aLen)
{
    seekg(aLen, std::ios_base::cur);
}


std::vector<uint8_t> DataStream::readBytes(size_t aLen)
{
    std::vector<uint8_t> data;
    data.resize(aLen);

    for(auto& byte : data)
    {
        byte = readUint8();
    }

    return data;
}


std::string DataStream::readStringZeroTerm()
{
    std::string str;

    const size_t max_chars = 400u;

    for(size_t i = 0u; i < max_chars; ++i)
    {
        char c = get();

        if(c != '\0')
        {
            str += c;
        }
        else
        {
            break;
        }
    }

    // @todo This check was added during testing to such that the parser fails when trying to
    //       interpret random data as string. Remove this check as soon as the parser works
    //       stable because strings can actually be very long.
    if(str.length() == max_chars)
    {
        const std::string msg = "Loop cancled because the string is unexpectedly large. More than "
                                 + std::to_string(max_chars) + " characters!";

        spdlog::error(msg);

        throw std::runtime_error(msg);
    }

    sanitizeNoEoF();

    return str;
}


std::string DataStream::readStringLenTerm()
{
    const uint16_t len = readUint16();

    const size_t max_chars = 400u;

    std::string str;

    for(size_t i = 0u; i < len && i < max_chars; ++i)
    {
        char c = get();

        if(c != '\0')
        {
            str += c;
        }
        else
        {
            const std::string msg = "Didn't expect null byte within string!";

            spdlog::error(msg);

            throw std::runtime_error(msg);
        }
    }

    // @todo This check was added during testing to such that the parser fails when trying to
    //       interpret random data as string. Remove this check as soon as the parser works
    //       stable because strings can actually be very long.
    if(str.length() == max_chars)
    {
        const std::string msg = "Loop cancled because the string is unexpectedly large. More than "
                                 + std::to_string(max_chars) + " characters!";

        spdlog::error(msg);

        throw std::runtime_error(msg);
    }

    sanitizeNoEoF();

    return str;
}


std::string DataStream::readStringLenZeroTerm()
{
    const uint16_t len = readUint16();

    const std::string str = readStringZeroTerm();

    if(str.length() != len)
    {
        const std::string msg = "Zero terminated string length (" + std::to_string(str.length())
                                 + ") does not match the preceding length ("
                                 + std::to_string(len) + ") definition!";

        spdlog::error(msg);

        throw std::runtime_error(msg);
    }

    sanitizeNoEoF();

    return str;
}


void DataStream::printUnknownData(size_t aLen, const std::string& aComment)
{
    const auto data = readBytes(aLen);

    if(aLen > 0u)
    {
        spdlog::info(aComment);
        spdlog::info(dataToStr(data));
    }
}


void DataStream::padRest(size_t aStartOffset, size_t aBlockSize, bool aPadIsZero)
{
    const size_t currOffset = getCurrentOffset();
    const size_t offsetDiff = currOffset - aStartOffset;

    if(offsetDiff > aBlockSize)
    {
        const std::string msg = "Already parsed " + std::to_string(offsetDiff)
                                 + " bytes but should have only been "
                                 + std::to_string(aBlockSize) + "!";

        spdlog::error(msg);

        throw std::runtime_error(msg);
    }

    const size_t paddingLen = aBlockSize - offsetDiff;

    if(aPadIsZero)
    {
        for(size_t i = 0u; i < paddingLen; ++i)
        {
            const uint8_t padByte = readUint8();

            if(padByte != 0x00u)
            {
                const std::string msg = "Padding byte is expected to be 0x00!";

                spdlog::error(msg);

                throw std::runtime_error(msg);
            }
        }
    }
    else
    {
        discardBytes(paddingLen);
        // printUnknownData(paddingLen, "Padding Bytes");
    }
}


std::string DataStream::getCurrentOffsetStrMsg()
{
    return fmt::format("Offset at 0x{:08x}", getCurrentOffset());
}


void DataStream::printData(const std::vector<uint8_t>& aData)
{
    spdlog::info(dataToStr(aData));
}


std::string DataStream::dataToStr(const std::vector<uint8_t>& aData)
{
    const unsigned int line_width = 16u;
    const std::string hex_spacing = " ";
    const std::string str_spacing = "";

    std::string preamble;
    std::string line_hex;
    std::string line_str;

    std::string output;

    std::string line;
    for(size_t i = 0u; i < aData.size(); ++i)
    {
        if(i % line_width == 0u)
        {
            preamble = fmt::format("0x{:08x}: ", getCurrentOffset());
        }

        char c = static_cast<char>(aData[i]);
        line_hex += fmt::format("{:02x}", aData[i]);
        line_str += std::isprint(c) ? c : '.';

        if((i + 1) % line_width == 0u)
        {
            output += preamble + line_hex + " | " + line_str + '\n';
            preamble.clear();
            line_hex.clear();
            line_str.clear();
        }
        else if(i == aData.size() - 1)
        {
            for(size_t i = 0u; i < line_width - (aData.size() % line_width); ++i)
            {
                line_hex += "  " + hex_spacing; // 2 spaces for 1 byte
                line_str += " "  + str_spacing; // 1 character for 1 byte
            }

            output += preamble + line_hex + " | " + line_str + '\n';
        }
        else
        {
            line_hex += hex_spacing;
            line_str += str_spacing;
        }
    }

    return output;
}


void DataStream::assumeData(const std::vector<uint8_t>& aExpectedData, const std::string& aComment)
{
    const std::vector<uint8_t> data = readBytes(aExpectedData.size());

    size_t ctr = 0u;

    const auto checkByte = [&ctr, &aExpectedData](uint8_t byte)
    {
        return byte == aExpectedData[ctr++];
    };

    if(!std::all_of(data.cbegin(), data.cend(), checkByte))
    {
        const std::string msg = "Assumption failed: " + aComment + '\n'
            + "Expected:\n" + dataToStr(aExpectedData) +
            + "but got:\n" + dataToStr(data);

        spdlog::error(msg);

        throw std::runtime_error(msg);
    }
}