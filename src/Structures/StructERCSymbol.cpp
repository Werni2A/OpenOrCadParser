#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "FutureData.hpp"
#include "General.hpp"
#include "Structures/StructERCSymbol.hpp"
#include "Structures/StructPrimitives.hpp"
#include "Structures/StructSymbolBBox.hpp"


void StructERCSymbol::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const std::optional<FutureData> thisFuture = getFutureData();

    // @todo Use readStructure()
    StructPrimitives primitives{mDs};
    primitives.read();

    mDs.get().printUnknownData(4, std::string(__func__) + " - 1");

    uint16_t len = mDs.get().readUint16();

    for(size_t i = 0u; i < len; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();

        readPrimitive(primitive);
    }

    // @todo readStructure();
    // @todo not sure if this belongs into this structure and how do we know whether it
    //       is used or not? (BBox should be optional according to XSD)
    //       Probably defined by prefix?
    readPreamble();

    StructSymbolBBox bbox{mDs};
    bbox.read();
    this->symbolBBox = bbox;

    sanitizeThisFutureSize(thisFuture);

    checkTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}