#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Color.hpp"
#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructPartInst.hpp"


void StructPartInst::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    // const std::optional<FutureData> thisFuture = getFutureData();

    mDs.get().printUnknownData(8, std::string(__func__) + " - 0");

    const std::string pkgName = mDs.get().readStringLenZeroTerm();

    spdlog::trace("pkgName = {}", pkgName);

    const uint32_t dbId = mDs.get().readUint32();

    spdlog::trace("dbId = {}", dbId);

    mDs.get().printUnknownData(8, std::string(__func__) + " - 1");

    const int16_t locX = mDs.get().readInt16();
    const int16_t locY = mDs.get().readInt16();

    spdlog::trace("locX = {}", locX);
    spdlog::trace("locY = {}", locY);

    const Color color = ToColor(mDs.get().readUint16()); // @todo educated guess

    spdlog::trace("color = {}", ::to_string(color));

    mDs.get().printUnknownData(2, std::string(__func__) + " - 2");

    const uint16_t len = mDs.get().readUint16();

    spdlog::trace("len = {}", len);

    for(size_t i = 0u; i < len; ++i)
    {
        spdlog::critical("VERIFYING StructPartInst Structure42 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
        // readStructure(); // @todo push struct
    }

    mDs.get().printUnknownData(1, std::string(__func__) + " - 3");

    const std::string reference = mDs.get().readStringLenZeroTerm();

    spdlog::trace("reference = {}", reference);

    mDs.get().printUnknownData(14, std::string(__func__) + " - 4");

    const uint16_t len2 = mDs.get().readUint16();

    spdlog::trace("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        spdlog::critical("VERIFYING StructPartInst Structure2 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
        // readStructure(); // @todo push struct
    }

    const std::string sth1 = mDs.get().readStringLenZeroTerm(); // @todo needs verification

    spdlog::trace("sth1 = {}", sth1);

    mDs.get().printUnknownData(2, std::string(__func__) + " - 5");

    // @todo implement type_prefix_very_long
    // mDs.get().printUnknownData(18, std::string(__func__) + " - 6");

    // auto_read_prefixes();

    // sanitizeThisFutureSize(thisFuture);

    // readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}