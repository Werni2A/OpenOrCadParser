#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "../Parser.hpp"
#include "Properties2.hpp"


Properties2 Parser::readProperties2()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    Properties2 obj;

    obj.name = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 0", __func__)); // Unknown but probably string

    obj.refDes = mDs.readStringLenZeroTerm();

    mDs.assumeData({0x00, 0x00, 0x00}, fmt::format("{}: 1", __func__)); // Unknown but probably string

    obj.footprint = mDs.readStringLenZeroTerm();

    obj.sectionCount = mDs.readUint16(); // @todo has this something to do with units? Or was this just bad naming from myself?

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}