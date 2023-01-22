#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructWireScalar.hpp"


// @note Moved to StructWire::read
// void StructWireScalar::read(FileFormatVersion /* aVersion */)
// {
//     spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

//     auto_read_prefixes();

//     readPreamble();

//     const std::optional<FutureData> thisFuture = getFutureData();

//     dbId = mDs.get().readUint32();

//     spdlog::debug("dbId = {}", dbId);

//     mDs.get().printUnknownData(4, std::string(__func__) + " - 0");

//     wireColor = ToColor(mDs.get().readUint32());

//     spdlog::debug("wireColor = {}", ::to_string(wireColor));

//     startX = mDs.get().readInt32();
//     startY = mDs.get().readInt32();

//     spdlog::debug("startX = {}", startX);
//     spdlog::debug("startY = {}", startY);

//     endX = mDs.get().readInt32();
//     endY = mDs.get().readInt32();

//     spdlog::debug("endX = {}", endX);
//     spdlog::debug("endY = {}", endY);

//     mDs.get().printUnknownData(1, std::string(__func__) + " - 1");

//     const uint16_t len0 = mDs.get().readUint16();

//     spdlog::debug("len0 = {}", len0);

//     for(size_t i = 0u; i < len0; ++i)
//     {
//         // @todo Read structure should be 'Alias'
//         readStructure(); // @todo push
//     }

//     const uint16_t len1 = mDs.get().readUint16();

//     spdlog::debug("len1 = {}", len1);

//     for(size_t i = 0u; i < len1; ++i)
//     {
//         // @todo Read structure should be Display Property
//         readStructure(); // @todo push
//     }

//     wireLineWidth = ToLineWidth(mDs.get().readUint32());
//     wireLineStyle = ToLineStyle(mDs.get().readUint32());

//     spdlog::debug("wireLineWidth = {} | wireLineStyle = {}",
//         ::to_string(wireLineWidth), ::to_string(wireLineStyle));

//     sanitizeThisFutureSize(thisFuture);

//     readOptionalTrailingFuture();

//     spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
//     spdlog::info(to_string());
// }