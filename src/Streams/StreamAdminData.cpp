#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamAdminData.hpp"

void StreamAdminData::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    uint8_t sth0 = ds.readUint8();

    // @todo only those two values have been observed until now
    if(sth0 != 0 && sth0 != 1)
    {
        mCtx.mLogger.warn("{}: Found new value. Check it out!", getMethodName(this, __func__));
    }

    ds.assumeData(
        {0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: Found actually useful data!", getMethodName(this, __func__)));

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}