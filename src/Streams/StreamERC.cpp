#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamERC.hpp"


void StreamERC::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ercSymbol = dynamic_pointer_cast<StructERCSymbol>(parser.readStructure());

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}