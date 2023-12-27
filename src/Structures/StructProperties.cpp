#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Structures/StructProperties.hpp"


void StructProperties::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::Properties, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    ref = ds.readStringLenZeroTerm();

    spdlog::trace("ref = {}", ref);

    const std::string some_str = ds.readStringLenZeroTerm();

    spdlog::trace("some_str = {}", some_str);

    localFutureLst.checkpoint();

    // @todo use enum for the view (normal/convert)
    const uint16_t viewNumber = ds.readUint16(); // @todo I assume that this is the amount of views
                                               // the symbol has. Typically 1 (.Normal) or maybe
                                               // 2 with (.Normal and .Convert)
                                               // @todo Add to obj

    spdlog::trace("viewNumber = {}", viewNumber);

    if(viewNumber == 1U) // Contains ".Normal"
    {
        normalName = ds.readStringLenZeroTerm();
    }

    if(viewNumber == 2U) // Contains ".Normal" and ".Convert"
    {
        normalName = ds.readStringLenZeroTerm();
        convertName = ds.readStringLenZeroTerm();
    }

    spdlog::trace("normalName  = {}", normalName);
    spdlog::trace("convertName = {}", convertName);

    if(viewNumber != 1U && viewNumber != 2U)
    {
        const std::string msg = fmt::format("viewNumber = {} but expected it to be 1 or 2!",
            viewNumber);

        spdlog::error(msg);
        throw std::runtime_error(msg);
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}