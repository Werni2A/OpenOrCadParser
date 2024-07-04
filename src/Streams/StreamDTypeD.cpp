#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamDTypeD.hpp"

void OOCP::StreamDTypeD::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    // File can be completely empty (size of 0 Byte)
    while(!ds.isEoF())
    {
        Type type;

        type.name          = ds.readStringLenZeroTerm();
        type.componentType = ToComponentType(ds.readUint16());

        types.push_back(type);
    }

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}