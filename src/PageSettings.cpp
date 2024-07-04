#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "PageSettings.hpp"

void PageSettings::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    createDateTime = static_cast<time_t>(ds.readUint32());
    modifyDateTime = static_cast<time_t>(ds.readUint32());

    mCtx.mLogger.trace("createDateTime = {}", DateTimeToStr(createDateTime));
    mCtx.mLogger.trace("modifyDateTime = {}", DateTimeToStr(modifyDateTime));

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 0");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 2");
    ds.printUnknownData(4, getMethodName(this, __func__) + ": 3");

    width  = ds.readUint32();
    height = ds.readUint32();

    mCtx.mLogger.trace("width  = {}", width);
    mCtx.mLogger.trace("height = {}", height);

    pinToPin = ds.readUint32();

    mCtx.mLogger.trace("pinToPin = {}", pinToPin);

    ds.printUnknownData(2, getMethodName(this, __func__) + ": 4");

    horizontalCount = ds.readUint16();
    verticalCount   = ds.readUint16();

    mCtx.mLogger.trace("horizontalCount = {}", horizontalCount);
    mCtx.mLogger.trace("verticalCount   = {}", verticalCount);

    // @todo Probably related to Spice simulations
    ds.printUnknownData(2, getMethodName(this, __func__) + ": 5");

    horizontalWidth = ds.readUint32();
    verticalWidth   = ds.readUint32();

    mCtx.mLogger.trace("horizontalWidth = {}", horizontalWidth);
    mCtx.mLogger.trace("verticalWidth   = {}", verticalWidth);

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

    mCtx.mLogger.trace("horizontalChar = {}", horizontalChar);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 18");

    horizontalAscending = ds.readUint32();

    mCtx.mLogger.trace("horizontalAscending = {}", horizontalAscending);

    verticalChar = ds.readUint32();

    mCtx.mLogger.trace("verticalChar = {}", verticalChar);

    ds.printUnknownData(4, getMethodName(this, __func__) + ": 19");

    verticalAscending = ds.readUint32();

    mCtx.mLogger.trace("verticalAscending = {}", verticalAscending);

    isMetric            = ds.readUint32();
    borderDisplayed     = ds.readUint32();
    borderPrinted       = ds.readUint32();
    gridRefDisplayed    = ds.readUint32();
    gridRefPrinted      = ds.readUint32();
    titleblockDisplayed = ds.readUint32();
    titleblockPrinted   = ds.readUint32();
    ansiGridRefs        = ds.readUint32(); // @todo In the XML it's always 1, maybe a bug in OrCad?

    mCtx.mLogger.trace("isMetric            = {}", isMetric);
    mCtx.mLogger.trace("borderDisplayed     = {}", borderDisplayed);
    mCtx.mLogger.trace("borderPrinted       = {}", borderPrinted);
    mCtx.mLogger.trace("gridRefDisplayed    = {}", gridRefDisplayed);
    mCtx.mLogger.trace("gridRefPrinted      = {}", gridRefPrinted);
    mCtx.mLogger.trace("titleblockDisplayed = {}", titleblockDisplayed);
    mCtx.mLogger.trace("titleblockPrinted   = {}", titleblockPrinted);
    mCtx.mLogger.trace("ansiGridRefs        = {}", ansiGridRefs);

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}