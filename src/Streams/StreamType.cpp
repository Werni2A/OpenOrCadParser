#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamType.hpp"


void StreamType::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // File can be completely empty (size of 0 Byte)
    while(!mDs.get().isEoF())
    {
        Type type;

        type.name = mDs.get().readStringLenZeroTerm();
        type.componentType = ToComponentType(mDs.get().readUint16());

        types.push_back(type);
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::debug(to_string());
}