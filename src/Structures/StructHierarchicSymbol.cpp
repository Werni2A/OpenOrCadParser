#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/StructHierarchicSymbol.hpp"
#include "Structures/StructPrimitives.hpp"


void StructHierarchicSymbol::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    StructPrimitives primitives{mCtx};
    primitives.read();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}