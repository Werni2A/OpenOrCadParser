#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "PageSettings.hpp"


void PageSettings::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    createDateTime = static_cast<time_t>(mDs.get().readUint32());
    modifyDateTime = static_cast<time_t>(mDs.get().readUint32());

    spdlog::trace("createDateTime = {}", DateTimeToStr(createDateTime));
    spdlog::trace("modifyDateTime = {}", DateTimeToStr(modifyDateTime));

    mDs.get().printUnknownData(4, std::string(__func__) + " - 0");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 1");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 2");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 3");

    width  = mDs.get().readUint32();
    height = mDs.get().readUint32();

    spdlog::trace("width  = {}", width);
    spdlog::trace("height = {}", height);

    pinToPin = mDs.get().readUint32();

    spdlog::trace("pinToPin = {}", pinToPin);

    mDs.get().printUnknownData(2, std::string(__func__) + " - 4");

    horizontalCount = mDs.get().readUint16();
    verticalCount   = mDs.get().readUint16();

    spdlog::trace("horizontalCount = {}", horizontalCount);
    spdlog::trace("verticalCount   = {}", verticalCount);

    mDs.get().printUnknownData(2, std::string(__func__) + " - 5");

    horizontalWidth = mDs.get().readUint32();
    verticalWidth   = mDs.get().readUint32();

    spdlog::trace("horizontalWidth = {}", horizontalWidth);
    spdlog::trace("verticalWidth   = {}", verticalWidth);

    mDs.get().printUnknownData(4, std::string(__func__) + " - 6");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 7");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 8");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 9");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 10");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 11");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 12");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 13");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 14");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 15");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 16");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 17");

    horizontalChar = mDs.get().readUint32();

    spdlog::trace("horizontalChar = {}", horizontalChar);

    mDs.get().printUnknownData(4, std::string(__func__) + " - 18");

    horizontalAscending = mDs.get().readUint32();

    spdlog::trace("horizontalAscending = {}", horizontalAscending);

    verticalChar   = mDs.get().readUint32();

    spdlog::trace("verticalChar = {}", verticalChar);

    mDs.get().printUnknownData(4, std::string(__func__) + " - 19");

    verticalAscending = mDs.get().readUint32();

    spdlog::trace("verticalAscending = {}", verticalAscending);

    isMetric            = mDs.get().readUint32();
    borderDisplayed     = mDs.get().readUint32();
    borderPrinted       = mDs.get().readUint32();
    gridRefDisplayed    = mDs.get().readUint32();
    gridRefPrinted      = mDs.get().readUint32();
    titleblockDisplayed = mDs.get().readUint32();
    titleblockPrinted   = mDs.get().readUint32();
    ansiGridRefs        = mDs.get().readUint32(); // @todo In the XML it's always 1, maybe a bug in OrCad?

    spdlog::trace("isMetric            = {}", isMetric);
    spdlog::trace("borderDisplayed     = {}", borderDisplayed);
    spdlog::trace("borderPrinted       = {}", borderPrinted);
    spdlog::trace("gridRefDisplayed    = {}", gridRefDisplayed);
    spdlog::trace("gridRefPrinted      = {}", gridRefPrinted);
    spdlog::trace("titleblockDisplayed = {}", titleblockDisplayed);
    spdlog::trace("titleblockPrinted   = {}", titleblockPrinted);
    spdlog::trace("ansiGridRefs        = {}", ansiGridRefs);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}