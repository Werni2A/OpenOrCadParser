#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Primitives/Point.hpp"

void Point::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    y = ds.readUint16();
    x = ds.readUint16();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}