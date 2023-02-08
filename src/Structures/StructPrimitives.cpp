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


void StructPrimitives::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(Structure::Primitives, localFutureLst);

    readPreamble();

    localFutureLst.checkpoint();

    name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    const std::string someStr791411 = ds.readStringLenZeroTerm();

    spdlog::trace("someStr791411 = {}", someStr791411);

    localFutureLst.checkpoint();

    ds.printUnknownData(4, fmt::format("{}: 2", getMethodName(this, __func__)));

    const uint16_t len0 = ds.readUint16();

    const size_t nextCheckpointPos = localFutureLst.getNextCheckpointPos().value_or(0U);

    spdlog::trace("len0 = {}", len0);

    for(size_t i = 0u; i < len0; ++i)
    {
        const Primitive primitive = readPrefixPrimitive();

        // @todo Hack to get SymbolVector working
        if(primitive == Primitive::SymbolVector)
        {
            ds.setCurrentOffset(ds.getCurrentOffset() - 1U);
        }

        primitives.push_back(readPrimitive(primitive));

        // @todo Sometimes there is trailing data after the primitives
        //       but I don't know how many bytes, therefore discard them
        //       until the next primitive occurs. There might be rare
        //       false positives
        for(int i = 0; i < 64 && ds.getCurrentOffset() < nextCheckpointPos; ++i)
        {
            const auto prefix = ds.peek(2);

            bool isPrimValid = true;

            try
            {
                ToPrimitive(prefix[0]);
            }
            catch(...)
            {
                isPrimValid = false;
            }

            if(prefix[0] == prefix[1] && isPrimValid)
            {
                break;
            }

            ds.printUnknownData(1U);
        }
    }

    if(ds.getCurrentOffset() < nextCheckpointPos)
        localFutureLst.readUntilNextFutureData();

    localFutureLst.checkpoint();

    const uint16_t lenSymbolPins = ds.readUint16();

    spdlog::trace("lenSymbolPins = {}", lenSymbolPins);

    if(aVersion == FileFormatVersion::B || aVersion == FileFormatVersion::D)
    {
        ds.printUnknownData(1, fmt::format("{}: 1", getMethodName(this, __func__)));
    }

    for(size_t i = 0u; i < lenSymbolPins; ++i)
    {
        symbolPins.push_back(dynamic_pointer_cast<StructSymbolPin>(readStructure()));

        if(aVersion == FileFormatVersion::C || aVersion == FileFormatVersion::D)
        {
            ds.printUnknownData(1, fmt::format("{}: 1", getMethodName(this, __func__)));
        }
    }

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0u; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    localFutureLst.checkpoint();

    auto genProp = StructGeneralProperties{mCtx.get()};
    genProp.read();
    generalProperties = genProp;

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}