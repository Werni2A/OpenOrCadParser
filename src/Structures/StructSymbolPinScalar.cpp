#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Structures/StructSymbolPinScalar.hpp"


// @note Moved to StructSymbolPin::read
// void StructSymbolPinScalar::read(FileFormatVersion /* aVersion */)
// {
//     spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

//     auto_read_prefixes();

//     readPreamble();

//     const std::optional<FutureData> thisFuture = getFutureData();

//     name = mDs.get().readStringLenZeroTerm();

//     startX = mDs.get().readInt32();
//     startY = mDs.get().readInt32();
//     hotptX = mDs.get().readInt32();
//     hotptY = mDs.get().readInt32();

//     pinShape = ToPinShape(mDs.get().readUint16());

//     mDs.get().printUnknownData(2, getMethodName(this, __func__) + ": 0");

//     portType = ToPortType(mDs.get().readUint32());

//     mDs.get().printUnknownData(4, getMethodName(this, __func__) + ": 1");

//     // @todo Type is StructSymbolDisplayProp
//     const uint16_t lenSymbolDisplayProps = mDs.get().readUint16();

//     spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

//     for(size_t i = 0U; i < lenSymbolDisplayProps; ++i)
//     {
//         spdlog::critical("VERIFYING StructSymbolPinScalar Structure0 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
//         // readStructure();
//     }

//     sanitizeThisFutureSize(thisFuture);

//     readOptionalTrailingFuture();

//     spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
//     spdlog::trace(to_string());
// }