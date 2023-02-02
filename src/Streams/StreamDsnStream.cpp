#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Streams/StreamDsnStream.hpp"


// @note Printing the content of this structure needs to be done after parsing
//       `Library`, because there are some references to the string lists.
//       I.e. parse `Library` first and later on `DsnStream` to avoid issues.
void StreamDsnStream::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    // @todo return the parsed nameValueMapping from within read_single_short_prefix()
    //       and save it in DsnStream. I.e. `Library guid` and `Time Format Index`
    // const Structure structure = read_prefixes(2);
    auto_read_prefixes(Structure::DsnStream, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    localFutureLst.readRestOfStructure();

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}