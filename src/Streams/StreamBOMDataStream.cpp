#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamBOMDataStream.hpp"

void StreamBOMDataStream::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    // File size calculated without this value
    const uint32_t expectedByteLen = ds.readUint32();

    // Byte length of the previous value
    const uint32_t LEN_SIZE{sizeof(expectedByteLen)};
    static_assert(LEN_SIZE == 4U);

    const uint32_t actualByteLen = static_cast<uint32_t>(fs::file_size(mCtx.mInputStream) - LEN_SIZE);

    if(actualByteLen != static_cast<std::size_t>(expectedByteLen))
    {
        mCtx.mLogger.error("Got {} Byte but expected {} Byte", actualByteLen, expectedByteLen);
    }

    const uint8_t MAGIC_NEXT_STRING_IDENTIFIER{0xf9};

    std::vector<std::string> strings;

    uint8_t byte = MAGIC_NEXT_STRING_IDENTIFIER;
    while(byte == MAGIC_NEXT_STRING_IDENTIFIER)
    {
        std::string tmpStr;

        do
        {
            byte = ds.readUint8();

            if(byte != MAGIC_NEXT_STRING_IDENTIFIER)
            {
                tmpStr += static_cast<char>(byte);
            }
        } while(!ds.isEoF() && byte != MAGIC_NEXT_STRING_IDENTIFIER);

        strings.push_back(tmpStr);
        mCtx.mLogger.trace("strings[{}] = `{}`", strings.size() - 1, strings.back());
    }

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}