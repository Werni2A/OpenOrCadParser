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
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    // @todo return the parsed nameValueMapping from within read_single_short_prefix()
    //       and save it in DsnStream. I.e. `Library guid` and `Time Format Index`
    // const Structure structure = read_prefixes(2);
    const Structure structure = auto_read_prefixes();

    if(structure != Structure::DsnStream)
    {
        throw std::runtime_error(fmt::format("{}: Unexpected Structure `{}`", __func__, ::to_string(structure)));
    }

    readPreamble();

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}