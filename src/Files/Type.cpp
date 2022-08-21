#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "Type.hpp"

#include "../Parser.hpp"


std::vector<Type> Parser::readType()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    std::vector<Type> types;

    // File can be completely empty (size of 0 byte)
    while(!mDs.isEoF())
    {
        Type type;

        type.name = mDs.readStringLenZeroTerm();
        type.componentType = ToComponentType(mDs.readUint16());

        types.push_back(type);
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));

    // @todo move to separate to_string method
    std::string str;

    for(size_t i = 0U; i < types.size(); ++i)
    {
        str += fmt::format("[{:>3}]:\n", i);
        str += indent(to_string(types[i]), 2);
    }

    spdlog::info(str);

    return types;
}