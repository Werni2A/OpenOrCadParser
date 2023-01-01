#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/LineStyle.hpp"
#include "Enums/LineWidth.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSymbolPinScalar.hpp"


StructSymbolPinScalar Parser::readStructSymbolPinScalar()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructSymbolPinScalar obj;

    obj.name = mDs.readStringLenZeroTerm();

    obj.startX = mDs.readInt32();
    obj.startY = mDs.readInt32();
    obj.hotptX = mDs.readInt32();
    obj.hotptY = mDs.readInt32();

    obj.pinShape = ToPinShape(mDs.readUint16());

    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    obj.portType = ToPortType(mDs.readUint32());

    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    const uint16_t struct_len = mDs.readUint16();

    for(size_t i = 0U; i < struct_len; ++i)
    {
        const Structure structure = auto_read_prefixes();

        if(structure != Structure::SymbolDisplayProp)
        {
            const std::string msg = fmt::format("{}: Expected {} but got {}",
                __func__, to_string(Structure::SymbolDisplayProp), to_string(structure));

            spdlog::error(msg);
        }

        readStructure(structure);
    }

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}