#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamHierarchy.hpp"


void StreamHierarchy::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        aVersion = predictVersion();
    }

    ds.printUnknownData(9, getMethodName(this, __func__) + ": 0");

    const std::string schematicName = ds.readStringLenZeroTerm();

    spdlog::trace("schematicName = {}", schematicName);

    ds.printUnknownData(9, getMethodName(this, __func__) + ": 1");

    const uint16_t lenNetDbIdMappings = ds.readUint16();

    spdlog::trace("lenNetDbIdMappings = {}", lenNetDbIdMappings);

    for(size_t i = 0u; i < lenNetDbIdMappings; ++i)
    {
        netDbIdMappings.push_back(dynamic_pointer_cast<StructNetDbIdMapping>(readStructure()));

        const uint32_t dbId = ds.readUint32();

        spdlog::trace("dbId = {}", dbId);

        const std::string name = ds.readStringLenZeroTerm(); // net name

        spdlog::trace("name = {}", name);
    }

    const uint16_t lenSthInHierarchy3 = ds.readUint16();

    spdlog::trace("lenSthInHierarchy3 = {}", lenSthInHierarchy3);

    for(size_t i = 0u; i < lenSthInHierarchy3; ++i)
    {
        sthInHierarchy3s.push_back(dynamic_pointer_cast<StructSthInHierarchy3>(readStructure()));

        ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.5");
    }

    const uint32_t lenT0x5bs = ((int)aVersion & 0x01) ? ds.readUint32() : ds.readUint16();

    spdlog::trace("lenT0x5bs = {}", lenT0x5bs);

    for(size_t i = 0u; i < lenT0x5bs; ++i)
    {
        t0x5bs.push_back(dynamic_pointer_cast<StructT0x5b>(readStructure()));

        ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.6");
    }

    if((int)aVersion & 0x04)
    {
        ds.printUnknownData(8, getMethodName(this, __func__) + ": 2.2");
    }

    const uint32_t lenSthInHierarchy1 = ((int)aVersion & 0x08) ? ds.readUint32() : ds.readUint16();

    spdlog::trace("lenSthInHierarchy1 = {}", lenSthInHierarchy1);

    for(size_t i = 0u; i < lenSthInHierarchy1; ++i)
    {
        sthInHierarchy1s.push_back(dynamic_pointer_cast<StructSthInHierarchy1>(readStructure()));

        // Is one of [8, 27]
        // ds.printUnknownData(8, getMethodName(this, __func__) + ": 3");
        ds.printUnknownData(27, getMethodName(this, __func__) + ": 3");

        const uint16_t lenSthInHierarchy2 = ds.readUint16();

        spdlog::trace("lenSthInHierarchy2 = {}", lenSthInHierarchy2);

        for(size_t i = 0u; i < lenSthInHierarchy2; ++i)
        {
            sthInHierarchy2s.push_back(dynamic_pointer_cast<StructSthInHierarchy2>(readStructure()));

            ds.printUnknownData(6, getMethodName(this, __func__) + ": 4");
        }

        ds.printUnknownData(10, getMethodName(this, __func__) + ": 5");
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}