#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Streams/StreamDsnStream.hpp"


// @note Printing the content of this structure needs to be done after parsing
//       `Library`, because there are some references to the string lists.
//       I.e. parse `Library` first and later on `DsnStream` to avoid issues.
StreamDsnStream Parser::readStreamDsnStream()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StreamDsnStream obj;

    // @todo return the parsed nameValueMapping from within read_single_short_prefix()
    //       and save it in DsnStream. I.e. `Library guid` and `Time Format Index`
    // const Structure structure = read_prefixes(2);
    const Structure structure = auto_read_prefixes();

    if(structure != Structure::DsnStream)
    {
        throw std::runtime_error(fmt::format("{}: Unexpected Structure `{}`", __func__, to_string(structure)));
    }

    const uint32_t optionalLen = readPreamble();

    if(optionalLen != 0U)
    {
        spdlog::warn("{}: Expected optionalLen to be 0 but it's {}", __func__, optionalLen);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}