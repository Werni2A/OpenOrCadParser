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
        int discard_ctr = 0;
        for(discard_ctr = 0; discard_ctr < 64 && ds.getCurrentOffset() < nextCheckpointPos; ++discard_ctr)
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

            ds.discardBytes(1U);
        }

        ds.setCurrentOffset(ds.getCurrentOffset() - discard_ctr);
        ds.printUnknownData(discard_ctr, getMethodName(this, __func__) + ": Mysterious Content");
    }

    // @todo Parts of it probably belong to the upper trailing data
    if(ds.getCurrentOffset() < nextCheckpointPos)
        localFutureLst.readUntilNextFutureData("See FuturData of StructPrimitives");

    localFutureLst.checkpoint();

    const uint16_t lenSymbolPins = ds.readUint16();

    spdlog::trace("lenSymbolPins = {}", lenSymbolPins);

    for(size_t i = 0u; i < lenSymbolPins; ++i)
    {
        // Skip pin
        // @todo But why? Should I add an empty SymbolPin to the vector
        //       to keep the index correct. What does it mean to skip
        //       pins? Are previous definitions reused?
        //       I've seen the usage of 0x00 for pins in convert view
        //       but I'm not sure if this was just a coincidence. See
        //       test/designs/ROHMUSDC/LapisDevBoard/DESIGN FILES/OrCAD Capture Schematics/library/stepperlib.olb
        //       ML610Q111
        if(std::vector<uint8_t>{0U} == ds.peek(1U))
        {
            ds.printUnknownData(1U, "Skipping Pin");
            continue;
        }

        symbolPins.push_back(dynamic_pointer_cast<StructSymbolPin>(readStructure()));
    }

    const uint16_t lenSymbolDisplayProps = ds.readUint16();

    spdlog::trace("lenSymbolDisplayProps = {}", lenSymbolDisplayProps);

    for(size_t i = 0u; i < lenSymbolDisplayProps; ++i)
    {
        symbolDisplayProps.push_back(dynamic_pointer_cast<StructSymbolDisplayProp>(readStructure()));
    }

    localFutureLst.checkpoint();

    spdlog::debug("Checking {} vs {}", localFutureLst.cbegin()->getStopOffset(), ds.getCurrentOffset());
    if(localFutureLst.cbegin()->getStopOffset() != ds.getCurrentOffset())
    {
        readPreamble();

        for(std::size_t i{0U}; i < std::size_t{4U}; ++i)
        {
            const std::string s = ds.readStringLenZeroTerm();
            spdlog::trace("s[{}] = {}", i, s);
        }

        if(localFutureLst.cbegin()->getStopOffset() != ds.getCurrentOffset())
        {
            ds.printUnknownData(2, getMethodName(this, __func__) + ": Interesting but weird bytes");
        }

        localFutureLst.checkpoint();
    }

    localFutureLst.sanitizeCheckpoints();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::trace(to_string());
}