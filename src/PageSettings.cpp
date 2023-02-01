#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "PageSettings.hpp"


void PageSettings::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    createDateTime = static_cast<time_t>(ds.readUint32());
    modifyDateTime = static_cast<time_t>(ds.readUint32());

    spdlog::trace("createDateTime = {}", DateTimeToStr(createDateTime));
    spdlog::trace("modifyDateTime = {}", DateTimeToStr(modifyDateTime));

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 0");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 2");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 3");

    width  = ds.readUint32();
    height = ds.readUint32();

    spdlog::trace("width  = {}", width);
    spdlog::trace("height = {}", height);

    pinToPin = ds.readUint32();

    spdlog::trace("pinToPin = {}", pinToPin);

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 4");

    horizontalCount = ds.readUint16();
    verticalCount   = ds.readUint16();

    spdlog::trace("horizontalCount = {}", horizontalCount);
    spdlog::trace("verticalCount   = {}", verticalCount);

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 5");

    horizontalWidth = ds.readUint32();
    verticalWidth   = ds.readUint32();

    spdlog::trace("horizontalWidth = {}", horizontalWidth);
    spdlog::trace("verticalWidth   = {}", verticalWidth);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 6");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 7");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 8");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 9");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 10");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 11");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 12");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 13");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 14");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 15");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 16");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 17");

    horizontalChar = ds.readUint32();

    spdlog::trace("horizontalChar = {}", horizontalChar);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 18");

    horizontalAscending = ds.readUint32();

    spdlog::trace("horizontalAscending = {}", horizontalAscending);

    verticalChar   = ds.readUint32();

    spdlog::trace("verticalChar = {}", verticalChar);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 19");

    verticalAscending = ds.readUint32();

    spdlog::trace("verticalAscending = {}", verticalAscending);

    isMetric            = ds.readUint32();
    borderDisplayed     = ds.readUint32();
    borderPrinted       = ds.readUint32();
    gridRefDisplayed    = ds.readUint32();
    gridRefPrinted      = ds.readUint32();
    titleblockDisplayed = ds.readUint32();
    titleblockPrinted   = ds.readUint32();
    ansiGridRefs        = ds.readUint32(); // @todo In the XML it's always 1, maybe a bug in OrCad?

    spdlog::trace("isMetric            = {}", isMetric);
    spdlog::trace("borderDisplayed     = {}", borderDisplayed);
    spdlog::trace("borderPrinted       = {}", borderPrinted);
    spdlog::trace("gridRefDisplayed    = {}", gridRefDisplayed);
    spdlog::trace("gridRefPrinted      = {}", gridRefPrinted);
    spdlog::trace("titleblockDisplayed = {}", titleblockDisplayed);
    spdlog::trace("titleblockPrinted   = {}", titleblockPrinted);
    spdlog::trace("ansiGridRefs        = {}", ansiGridRefs);

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}