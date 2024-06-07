#ifndef DATASTREAM_HPP
#define DATASTREAM_HPP


#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"


class FutureDataLst;
class Record;
class StreamContext;


namespace fs = std::filesystem;


class DataStream : public std::ifstream
{
public:

    // DataStream() : std::ifstream{}
    // { }

    DataStream(const fs::path& aFile, StreamContext& aCtx) : std::ifstream{aFile, std::ifstream::binary}, mCtx{aCtx}
    { }

    // Checks whether the stream has reached the end of the file.
    // I.e. reading another Byte would result in an error.
    bool isEoF()
    {
        return std::ifstream::peek() == EOF;
    }

    // Throws if data beyond end of file was read.
    void sanitizeNoEoF()
    {
        if(eof())
        {
            throw std::runtime_error("Read beyond EoF!");
        }
    }

    // Throws if not at EoF.
    void sanitizeEoF()
    {
        if(!isEoF())
        {
            throw std::runtime_error("Expected EoF but did not reach it!");
        }
    }

    std::vector<uint8_t> peek(size_t aLen)
    {
        std::vector<uint8_t> data;
        data.resize(aLen);

        const size_t startOffset = getCurrentOffset();

        for(auto& byte : data)
        {
            byte = get();
        }

        setCurrentOffset(startOffset);

        return data;
    }

    /**
     * @brief Set the current file stream offset.
     *
     * @param aOffset New offset for the file stream.
     *
     * @return size_t Old offset before the new one was applied.
     */
    size_t setCurrentOffset(size_t aOffset)
    {
        const auto oldOffset = tellg();
        clear();
        seekg(aOffset, std::ios::beg);

        return oldOffset;
    }

    /**
     * @brief Get the offset from the beginning of
     *        the file to the current parsing position.
     *
     * @return size_t Offset.
     */
    size_t getCurrentOffset()
    {
        return tellg();
    }

    int8_t readInt8()
    {
        int8_t val;
        read(reinterpret_cast<char*>(&val), 1);

        sanitizeNoEoF();

        return val;
    }

    int16_t readInt16()
    {
        int16_t val;
        read(reinterpret_cast<char*>(&val), 2);

        sanitizeNoEoF();

        return val;
    }

    int32_t readInt32()
    {
        int32_t val;
        read(reinterpret_cast<char*>(&val), 4);

        sanitizeNoEoF();

        return val;
    }

    uint8_t readUint8()
    {
        uint8_t val;
        read(reinterpret_cast<char*>(&val), 1);

        sanitizeNoEoF();

        return val;
    }

    uint16_t readUint16()
    {
        uint16_t val;
        read(reinterpret_cast<char*>(&val), 2);

        sanitizeNoEoF();

        return val;
    }

    uint32_t readUint32()
    {
        uint32_t val;
        read(reinterpret_cast<char*>(&val), 4);

        sanitizeNoEoF();

        return val;
    }

    void discardBytes(size_t aLen);

    std::vector<uint8_t> readBytes(size_t aLen);

    /**
     * @brief Read string that has null byte termination.
     *
     * @return std::string Read text.
     */
    std::string readStringZeroTerm();

    /**
     * @brief Read string that has preceding length specification
     *        but no terminating null byte.
     *
     * @note Length specification uses a uint16_t.
     *
     * @return std::string Read text.
     */
    std::string readStringLenTerm();

    /**
     * @brief Read string that has preceding length definition and
     *        is additionally zero terminated.
     *
     * @note Length specification uses a uint16_t.
     *
     * @return std::string Read text.
     */
    std::string readStringLenZeroTerm();

    void printUnknownData(size_t aLen, const std::string& aComment = "");

    /**
     * @brief Pad stream such that the whole block size is read.
     *
     * @param aStartOffset Offset where the block size is counted from.
     * @param aBlockSize Size of the whole block that is padded.
     * @param aPadIsZero true  = Padding bytes must be 0x00.
     *                   false = Padding bytes can take any value.
     */
    void padRest(size_t aStartOffset, size_t aBlockSize, bool aPadIsZero = true);

    std::string getCurrentOffsetStrMsg();

    void printData(const std::vector<uint8_t>& aData);

    std::string dataToStr(const std::vector<uint8_t>& aData);

    void assumeData(const std::vector<uint8_t>& aExpectedData, const std::string& aComment = "");

    StreamContext& mCtx;
};


#endif // DATASTREAM_HPP