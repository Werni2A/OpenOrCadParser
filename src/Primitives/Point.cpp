#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Primitives/Point.hpp"


void Point::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    y = ds.readUint16();
    x = ds.readUint16();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}