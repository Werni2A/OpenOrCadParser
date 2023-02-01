#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "PageSettings.hpp"
#include "Streams/StreamPage.hpp"


void StreamPage::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    auto_read_prefixes(localFutureLst);
    // read_prefixes(3, localFutureLst);

    readPreamble();

    name = ds.readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    pageSize = ds.readStringLenZeroTerm();

    spdlog::trace("pageSize = {}", pageSize);

    pageSettings.read();

    // @todo Contains StructTitleBlock
    const uint16_t lenA = ds.readUint16();

    spdlog::trace("lenA = {}", lenA);

    for(size_t i = 0u; i < lenA; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page StructureA is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    const uint16_t lenT0x34s = ds.readUint16();

    spdlog::trace("lenT0x34s = {}", lenT0x34s);

    for(size_t i = 0u; i < lenT0x34s; ++i)
    {
        t0x34s.push_back(dynamic_pointer_cast<StructT0x34>(readStructure()));
    }

    const uint16_t lenT0x35s = ds.readUint16();

    spdlog::trace("lenT0x35s = {}", lenT0x35s);

    for(size_t i = 0u; i < lenT0x35s; ++i)
    {
        t0x35s.push_back(dynamic_pointer_cast<StructT0x35>(readStructure()));
    }

    const uint16_t lenB = ds.readUint16();

    spdlog::trace("lenB = {}", lenB);

    for(size_t i = 0; i < lenB; ++i)
    {
        const std::string net = ds.readStringLenZeroTerm();
        const uint32_t id = ds.readUint32();

        spdlog::trace("net = {}", net);
        spdlog::trace("id  = {}", id);
    }

    const uint16_t lenWires = ds.readUint16();

    spdlog::trace("lenWires = {}", lenWires);

    for(size_t i = 0u; i < lenWires; ++i)
    {
        wires.push_back(dynamic_pointer_cast<StructWire>(readStructure()));
    }

    const uint16_t lenPartInsts = ds.readUint16();

    spdlog::trace("lenPartInsts = {}", lenPartInsts);

    for(size_t i = 0u; i < lenPartInsts; ++i)
    {
        partInsts.push_back(dynamic_pointer_cast<StructPartInst>(readStructure()));
    }

    const uint16_t lenPorts = ds.readUint16();

    spdlog::trace("lenPorts = {}", lenPorts);

    for(size_t i = 0u; i < lenPorts; ++i)
    {
        ports.push_back(dynamic_pointer_cast<StructPort>(readStructure()));
    }

    // @todo They are StructureGlobal
    const uint16_t len5 = ds.readUint16();

    spdlog::trace("len5 = {}", len5);

    for(size_t i = 0u; i < len5; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page Structure5 is {}", NAMEOF_TYPE_RTTI(*s));
        }

        ds.printUnknownData(5, fmt::format("{}: 0", getMethodName(this, __func__)));
    }

    // @todo They are StructureOffPageConnector
    const uint16_t len6 = ds.readUint16();

    spdlog::trace("len6 = {}", len6);

    for(size_t i = 0u; i < len6; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page Structure6 is {}", NAMEOF_TYPE_RTTI(*s));
        }

        ds.printUnknownData(5, fmt::format("{}: 0", getMethodName(this, __func__)));
    }

    const uint16_t len7 = ds.readUint16();

    spdlog::trace("len7 = {}", len7);

    for(size_t i = 0u; i < len7; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page Structure7 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    const uint16_t len8 = ds.readUint16();

    spdlog::trace("len8 = {}", len8);

    for(size_t i = 0u; i < len8; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page Structure8 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    const uint16_t lenGraphicInsts = ds.readUint16();

    spdlog::trace("lenGraphicInsts = {}", lenGraphicInsts);

    for(size_t i = 0u; i < lenGraphicInsts; ++i)
    {
        graphicInsts.push_back(dynamic_pointer_cast<StructGraphicInst>(readStructure()));
    }

    const uint16_t len10 = ds.readUint16();

    spdlog::trace("len10 = {}", len10);

    for(size_t i = 0u; i < len10; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page Structure10 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    const uint16_t len11 = ds.readUint16();

    spdlog::trace("len11 = {}", len11);

    for(size_t i = 0u; i < len11; ++i)
    {
        // @todo push structure
        const auto s = readStructure();
        if(s)
        {
            spdlog::debug("VERIFYING Page Structure11 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    // localFutureLst.readRestOfStructure();

    // // localFutureLst.checkpoint(ds.getCurrentOffset());

    // // localFutureLst.sanitizeNoFutureDataLeft();

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}