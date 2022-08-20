#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "DsnStream.hpp"

#include "../Parser.hpp"
#include "../Enums/Structure.hpp"

// @note Printing the content of this structure needs to be done after parsing
//       `Library`, because there are some references to the string lists.
//       I.e. parse `Library` first and later on `DsnStream` to avoid issues.
DsnStream Parser::readDsnStream()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    DsnStream obj;

    // @todo return the parsed nameValueMapping from within read_type_prefix_short
    //       and save it in DsnStream. I.e. `Library guid` and `Time Format Index`
    const Structure type = read_type_prefix_short();

    if(type != Structure::DsnStream)
    {
        throw std::runtime_error(fmt::format("{}: Unexpected Structure `{}`", __func__, to_string(type)));
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