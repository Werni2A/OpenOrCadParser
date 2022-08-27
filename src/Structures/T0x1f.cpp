#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../Enums/LineStyle.hpp"
#include "../Enums/LineWidth.hpp"
#include "../General.hpp"
#include "T0x1f.hpp"

#include "../Parser.hpp"


// @todo Probably specifies the 'Package Properties'
T0x1f Parser::readT0x1f()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    T0x1f obj;

    obj.name = mDs.readStringLenZeroTerm();

    std::string unknownStr0 = mDs.readStringLenZeroTerm(); // @todo figure out
    spdlog::debug("{}: unknownStr0 = {}", __func__, unknownStr0);

    obj.refDes = mDs.readStringLenZeroTerm();

    std::string unknownStr1 = mDs.readStringLenZeroTerm(); // @todo figure out
    spdlog::debug("{}: unknownStr1 = {}", __func__, unknownStr1);

    obj.pcbFootprint = mDs.readStringLenZeroTerm();

    // Maybe the last two bytes specify the amount of units the symbols has?
    // Also called "Section Count"
    // Is Len of outter structure, comment out
    mDs.printUnknownData(2, std::string(__func__) + " - 0 - Prob. Unit Count");

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}