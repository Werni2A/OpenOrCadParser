#ifndef DATASTREAM_H
#define DATASTREAM_H


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


namespace fs = std::filesystem;


class DataStream : public std::ifstream
{
public:

    DataStream() : std::ifstream{}
    { }

    DataStream(const fs::path& aFile) : std::ifstream{aFile, std::ifstream::binary}
    { }

    bool isEoF()
    {
        return std::ifstream::peek() == EOF;
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
        return val;
    }

    int16_t readInt16()
    {
        int16_t val;
        read(reinterpret_cast<char*>(&val), 2);
        return val;
    }

    int32_t readInt32()
    {
        int32_t val;
        read(reinterpret_cast<char*>(&val), 4);
        return val;
    }

    uint8_t readUint8()
    {
        uint8_t val;
        read(reinterpret_cast<char*>(&val), 1);
        return val;
    }

    uint16_t readUint16()
    {
        uint16_t val;
        read(reinterpret_cast<char*>(&val), 2);
        return val;
    }

    uint32_t readUint32()
    {
        uint32_t val;
        read(reinterpret_cast<char*>(&val), 4);
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
     * @brief Read string that has preceeding length specification
     *        but no terminating null byte.
     *
     * @note Length specification uses a uint16_t.
     *
     * @return std::string Read text.
     */
    std::string readStringLenTerm();

    /**
     * @brief Read string that has preceeding length definition and
     *        is additionally zero terminated.
     *
     * @note Length specification uses a uint16_t.
     *
     * @return std::string Read text.
     */
    std::string readStringLenZeroTerm();

    std::ostream& printUnknownData(std::ostream& aOs, size_t aLen, const std::string& aComment = "");

    /**
     * @brief Pad stream such that the whole block size is read.
     *
     * @param aStartOffset Offset where the block size is counted from.
     * @param aBlockSize Size of the whole block that is padded.
     * @param aPadIsZero true  = Padding bytes must be 0x00.
     *                   false = Padding bytes can take any value.
     */
    void padRest(size_t aStartOffset, size_t aBlockSize, bool aPadIsZero = true);

    std::ostream& printCurrentOffset(std::ostream& aOs);

    std::ostream& printData(std::ostream& aOs, const std::vector<uint8_t>& aData);

    void assumeData(const std::vector<uint8_t>& aExpectedData, const std::string& aComment = "");
};


#endif // DATASTREAM_H