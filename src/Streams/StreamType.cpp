#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamType.hpp"


void StreamType::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    // File can be completely empty (size of 0 Byte)
    while(!ds.isEoF())
    {
        Type type;

        type.name = ds.readStringLenZeroTerm();
        type.componentType = ToComponentType(ds.readUint16());

        types.push_back(type);
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}