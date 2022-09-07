#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "FutureData.hpp"
#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructERCSymbol.hpp"


StructERCSymbol Parser::readStructERCSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    StructERCSymbol obj;

    std::string name = mDs.readStringLenZeroTerm();

    // @todo Probably 'sourceLibName' which is a string but I'm not sure. Could also be the
    //       last part of the next unknown block
    mDs.printUnknownData(3, std::string(__func__) + " - 0");

    sanitizeThisFutureSize(thisFuture);

    // @todo move the following part out of this method?
    mDs.printUnknownData(4, std::string(__func__) + " - 1");

    uint16_t len = mDs.readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        Primitive geometryStructure1 = ToPrimitive(mDs.readUint8());
        Primitive geometryStructure2 = ToPrimitive(mDs.readUint8());

        if(geometryStructure1 != geometryStructure2)
        {
            throw std::runtime_error("Geometry structures should be equal!");
        }

        readPrimitive(geometryStructure1); // @todo add to obj
    }

    // @todo not sure if this belongs into this structure and how do we know whether it
    //       is used or not? (BBox should be optional according to XSD)
    //       Probably defined by prefix?
    readPreamble();
    readStructSymbolBBox(); // @todo push structure

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}