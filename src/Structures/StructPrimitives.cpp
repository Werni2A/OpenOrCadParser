#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Structures/StructPrimitives.hpp"


void StructPrimitives::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::Primitives, localFutureLst);

    readPreamble();

    name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    const std::string someStr = ds.readStringLenZeroTerm();

    spdlog::trace("someStr = {}", someStr);

    ds.printUnknownData(4);

    // const uint16_t len0 = ds.readUint16();

    // spdlog::trace("len0 = {}", len0);

    // for(size_t i = 0u; i < len0; ++i)
    // {
    //     const Primitive primitive = readPrefixPrimitive();

    //     readPrimitive(primitive);
    // }


    // const uint16_t lenSymbolPins = ds.readUint16();

    // spdlog::trace("lenSymbolPins = {}", lenSymbolPins);

    // for(size_t i = 0u; i < lenSymbolPins; ++i)
    // {
    //     symbolPins.push_back(dynamic_pointer_cast<StructSymbolPin>(readStructure()));

    //     // @todo This hack should probably be moved into StructSymbolPin
    //     const uint8_t early_out = ds.peek(1)[0];
    //     spdlog::debug("early_out = {}", early_out);

    //     if(early_out == 0U)
    //     {
    //         // @todo does not always occur, even in the same file. Maybe its some byte alignment?
    //         ds.printUnknownData(1, fmt::format("{}: Early Out Indicator",
    //             getMethodName(this, __func__)));
    //         break;
    //     }
    // }

    // const uint16_t lenSymbolDisplayProps = ds.readUint16();

    // spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    // for(size_t i = 0u; i < lenSymbolDisplayProps; ++i)
    // {
    //     symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    // }

    // const std::string someStr0 = ds.readStringLenZeroTerm();
    // const std::string someStr1 = ds.readStringLenZeroTerm(); // @todo Maybe incorrect
    // const std::string someStr2 = ds.readStringLenZeroTerm();
    // const std::string someStr3 = ds.readStringLenZeroTerm();

    // spdlog::trace("someStr0 = {}", someStr0);
    // spdlog::trace("someStr1 = {}", someStr1);
    // spdlog::trace("someStr2 = {}", someStr2);
    // spdlog::trace("someStr3 = {}", someStr3);

    // ds.printUnknownData(2, fmt::format("{}: 0", getMethodName(this, __func__)));

    localFutureLst.readRestOfStructure();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}