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

    while(!ds.isEoF())
    {
        ds.printUnknownData(9, getMethodName(this, __func__) + ": 0");

        const std::string schematicName = ds.readStringLenZeroTerm();

        spdlog::trace("schematicName = {}", schematicName);

        const std::string s0 = ds.readStringLenZeroTerm();

        spdlog::trace("s0 = {}", s0);

        ds.printUnknownData(4, getMethodName(this, __func__) + ": 1.0");

        // size_t discardBytes = discard_until_auto_read_prefix_preamble();
        // size_t lenSize = 2;
        // if(discardBytes > lenSize)
        // {
        //     ds.printUnknownData(discardBytes - lenSize, "Discard in front of SthInHierarchy2");
        // }

        const uint16_t lenSomeHierarchyBase = ds.readUint16();

        spdlog::trace("lenSomeHierarchyBase = {}", lenSomeHierarchyBase);

        for(size_t i = 0u; i < lenSomeHierarchyBase; ++i)
        {
            spdlog::trace("Iteration of SomeHierarchyBase = {}", i);

            someHierarchyBases.push_back(dynamic_pointer_cast<StructSomeHierarchyBase>(readStructure()));

            ds.printUnknownData(6, getMethodName(this, __func__) + ": 4");
        }

        // size_t discardBytes = discard_until_auto_read_prefix_preamble();
        // size_t lenSize = 2;
        // if(discardBytes > lenSize)
        // {
        //     ds.printUnknownData(discardBytes - lenSize, "Discard in front of T0x5b");
        // }

        const uint16_t lenNetDbIdMappings = ds.readUint16();

        spdlog::trace("lenNetDbIdMappings = {}", lenNetDbIdMappings);

        for(size_t i = 0u; i < lenNetDbIdMappings; ++i)
        {
            spdlog::trace("Iteration of NetDbIdMappings = {}", i);

            netDbIdMappings.push_back(dynamic_pointer_cast<StructNetDbIdMapping>(readStructure()));

            const uint32_t dbId = ds.readUint32();

            spdlog::trace("dbId = {}", dbId);

            const std::string name = ds.readStringLenZeroTerm(); // net name

            spdlog::trace("name = {}", name);
        }

        // size_t discardBytes = discard_until_auto_read_prefix_preamble();
        // size_t lenSize = 2;
        // if(discardBytes > lenSize)
        // {
        //     ds.printUnknownData(discardBytes - lenSize, "Discard in front of T0x5b");
        // }

        const uint16_t lenSthInHierarchy3 = ds.readUint16();

        spdlog::trace("lenSthInHierarchy3 = {}", lenSthInHierarchy3);

        for(size_t i = 0u; i < lenSthInHierarchy3; ++i)
        {
            spdlog::trace("Iteration of SthInHierarchy3 = {}", i);

            sthInHierarchy3s.push_back(dynamic_pointer_cast<StructSthInHierarchy3>(readStructure()));

            ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.5");
        }

        // size_t discardBytes = discard_until_auto_read_prefix_preamble();
        // size_t lenSize = ((int)aVersion & 0x01 ? 2 : 4);
        // if(discardBytes > lenSize)
        // {
        //     ds.printUnknownData(discardBytes - lenSize, "Discard in front of T0x5b");
        // }

        // if((int)aVersion & 0x08)
        {
            const uint32_t lenT0x5bs = ((int)aVersion & 0x01) ? ds.readUint16() : ds.readUint32();

            spdlog::trace("lenT0x5bs = {}", lenT0x5bs);

            for(size_t i = 0u; i < lenT0x5bs; ++i)
            {
                spdlog::trace("Iteration of StructT0x5b = {}", i);

                t0x5bs.push_back(dynamic_pointer_cast<StructT0x5b>(readStructure()));

                ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.6");
            }

            if((int)aVersion & 0x08)
            {
                ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.7");
            }
        }

        // size_t discardBytes = discard_until_auto_read_prefix_preamble();
        // size_t lenSize = 2;
        // if(discardBytes > lenSize)
        // {
        //     ds.printUnknownData(discardBytes - lenSize, "Discard in front of SthInHierarchy1");
        // }

        if((int)aVersion & 0x04)
        {
            if((int)aVersion & 0x02)
            {
                ds.printUnknownData(2, getMethodName(this, __func__) + ": 1.8");
            }

            ds.printUnknownData(12, getMethodName(this, __func__) + ": 1.9");
        }

        ds.printUnknownData(2, getMethodName(this, __func__) + ": 1.95");

        if(ds.isEoF())
        {
            break;
        }

        sthInHierarchy1s.push_back(dynamic_pointer_cast<StructSthInHierarchy1>(readStructure()));

        ds.printUnknownData(8, getMethodName(this, __func__) + ": 3");
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}