#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamDsnStream.hpp"


// @note Printing the content of this structure needs to be done after parsing
//       `Library`, because there are some references to the string lists.
//       I.e. parse `Library` first and later on `DsnStream` to avoid issues.
void StreamDsnStream::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    // @todo Extract in separate structure parser
    // @todo return the parsed nameValueMapping from within read_single_short_prefix()
    //       and save it in DsnStream. I.e. `Library guid` and `Time Format Index`
    // const Structure structure = read_prefixes(2);
    parser.auto_read_prefixes(Structure::DsnStream, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    localFutureLst.readRestOfStructure();

    // @todo Add checkpoint sanitization
    // localFutureLst.sanitizeCheckpoints();

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}