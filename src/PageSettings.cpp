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

    spdlog::debug("createDateTime = {}", DateTimeToStr(createDateTime));
    spdlog::debug("modifyDateTime = {}", DateTimeToStr(modifyDateTime));

    mDs.get().printUnknownData(4, std::string(__func__) + " - 0");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 1");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 2");
    mDs.get().printUnknownData(4, std::string(__func__) + " - 3");

    width  = mDs.get().readUint32();
    height = mDs.get().readUint32();

    spdlog::debug("width  = {}", width);
    spdlog::debug("height = {}", height);

    pinToPin = mDs.get().readUint32();

    spdlog::debug("pinToPin = {}", pinToPin);

    mDs.get().printUnknownData(2, std::string(__func__) + " - 4");

    horizontalCount = mDs.get().readUint16();
    verticalCount   = mDs.get().readUint16();

    spdlog::debug("horizontalCount = {}", horizontalCount);
    spdlog::debug("verticalCount   = {}", verticalCount);

    mDs.get().printUnknownData(2, std::string(__func__) + " - 5");

    horizontalWidth = mDs.get().readUint32();
    verticalWidth   = mDs.get().readUint32();

    spdlog::debug("horizontalWidth = {}", horizontalWidth);
    spdlog::debug("verticalWidth   = {}", verticalWidth);

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

    spdlog::debug("horizontalChar = {}", horizontalChar);

    mDs.get().printUnknownData(4, std::string(__func__) + " - 18");

    horizontalAscending = mDs.get().readUint32();

    spdlog::debug("horizontalAscending = {}", horizontalAscending);

    verticalChar   = mDs.get().readUint32();

    spdlog::debug("verticalChar = {}", verticalChar);

    mDs.get().printUnknownData(4, std::string(__func__) + " - 19");

    verticalAscending = mDs.get().readUint32();

    spdlog::debug("verticalAscending = {}", verticalAscending);

    isMetric            = mDs.get().readUint32();
    borderDisplayed     = mDs.get().readUint32();
    borderPrinted       = mDs.get().readUint32();
    gridRefDisplayed    = mDs.get().readUint32();
    gridRefPrinted      = mDs.get().readUint32();
    titleblockDisplayed = mDs.get().readUint32();
    titleblockPrinted   = mDs.get().readUint32();
    ansiGridRefs        = mDs.get().readUint32(); // @todo In the XML it's always 1, maybe a bug in OrCad?

    spdlog::debug("isMetric            = {}", isMetric);
    spdlog::debug("borderDisplayed     = {}", borderDisplayed);
    spdlog::debug("borderPrinted       = {}", borderPrinted);
    spdlog::debug("gridRefDisplayed    = {}", gridRefDisplayed);
    spdlog::debug("gridRefPrinted      = {}", gridRefPrinted);
    spdlog::debug("titleblockDisplayed = {}", titleblockDisplayed);
    spdlog::debug("titleblockPrinted   = {}", titleblockPrinted);
    spdlog::debug("ansiGridRefs        = {}", ansiGridRefs);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}