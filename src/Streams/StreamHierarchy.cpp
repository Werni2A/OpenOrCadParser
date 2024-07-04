#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamHierarchy.hpp"

void OOCP::StreamHierarchy::read(FileFormatVersion aVersion)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    if(aVersion == FileFormatVersion::Unknown)
    {
        const auto predictionFunc = [this](FileFormatVersion aVersion) { this->read(aVersion); };
        aVersion                  = parser.predictVersion(predictionFunc);
    }

    ds.printUnknownData(9, getMethodName(this, __func__) + ": 0");

    const std::string schematicName = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("schematicName = {}", schematicName);

    ds.printUnknownData(7U, getMethodName(this, __func__) + ": 1");

    const uint16_t lenSthInHierarchy2s = ds.readUint16();

    mCtx.mLogger.trace("lenSthInHierarchy2s = {}", lenSthInHierarchy2s);

    for(std::size_t i{0U}; i < lenSthInHierarchy2s; ++i)
    {
        sthInHierarchy2s.push_back(dynamic_pointer_cast<StructSthInHierarchy2>(parser.readStructure()));

        ds.printUnknownData(4U, getMethodName(this, __func__) + ": 1.53");

        const std::string someName = ds.readStringLenZeroTerm();

        mCtx.mLogger.trace("someName = {}", someName);
    }

    const uint16_t lenNetDbIdMappings = ds.readUint16();

    mCtx.mLogger.trace("lenNetDbIdMappings = {}", lenNetDbIdMappings);

    for(size_t i = 0u; i < lenNetDbIdMappings; ++i)
    {
        netDbIdMappings.push_back(dynamic_pointer_cast<StructNetDbIdMapping>(parser.readStructure()));

        const uint32_t dbId = ds.readUint32();

        mCtx.mLogger.trace("dbId = {}", dbId);

        const std::string name = ds.readStringLenZeroTerm(); // net name

        mCtx.mLogger.trace("name = {}", name);
    }

    const uint16_t lenSthInHierarchy3 = ds.readUint16();

    mCtx.mLogger.trace("lenSthInHierarchy3 = {}", lenSthInHierarchy3);

    for(size_t i = 0u; i < lenSthInHierarchy3; ++i)
    {
        sthInHierarchy3s.push_back(dynamic_pointer_cast<StructSthInHierarchy3>(parser.readStructure()));

        ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.5");
    }

    const uint32_t lenT0x5bs = ((int)aVersion & 0x01) ? ds.readUint32() : ds.readUint16();

    mCtx.mLogger.trace("lenT0x5bs = {}", lenT0x5bs);

    for(size_t i = 0u; i < lenT0x5bs; ++i)
    {
        t0x5bs.push_back(dynamic_pointer_cast<StructT0x5b>(parser.readStructure()));

        ds.printUnknownData(8, getMethodName(this, __func__) + ": 1.6");
    }

    if((int)aVersion & 0x04)
    {
        ds.printUnknownData(8, getMethodName(this, __func__) + ": 2.2");
    }

    const uint32_t lenSthInHierarchy1 = ((int)aVersion & 0x08) ? ds.readUint32() : ds.readUint16();

    mCtx.mLogger.trace("lenSthInHierarchy1 = {}", lenSthInHierarchy1);

    for(size_t i = 0u; i < lenSthInHierarchy1; ++i)
    {
        sthInHierarchy1s.push_back(dynamic_pointer_cast<StructSthInHierarchy1>(parser.readStructure()));

        // Is one of [8, 27]
        // ds.printUnknownData(8, getMethodName(this, __func__) + ": 3");
        ds.printUnknownData(27, getMethodName(this, __func__) + ": 3");

        const uint16_t lenSomeHierarchyBase = ds.readUint16();

        mCtx.mLogger.trace("lenSomeHierarchyBase = {}", lenSomeHierarchyBase);

        for(size_t i = 0u; i < lenSomeHierarchyBase; ++i)
        {
            someHierarchyBases.push_back(dynamic_pointer_cast<StructSomeHierarchyBase>(parser.readStructure()));

            ds.printUnknownData(6, getMethodName(this, __func__) + ": 4");
        }

        ds.printUnknownData(10, getMethodName(this, __func__) + ": 5");
    }

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}