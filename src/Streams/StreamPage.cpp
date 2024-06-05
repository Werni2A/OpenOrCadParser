#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "GenericParser.hpp"
#include "PageSettings.hpp"
#include "Streams/StreamPage.hpp"


void StreamPage::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    // @todo Extract in separate structure parser
    parser.auto_read_prefixes(Structure::Page, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    name = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("name = {}", name);

    pageSize = ds.readStringLenZeroTerm();

    mCtx.mLogger.trace("pageSize = {}", pageSize);

    pageSettings.read();

    const uint16_t lenTitleBlocks = ds.readUint16();

    mCtx.mLogger.trace("lenTitleBlocks = {}", lenTitleBlocks);

    for(size_t i = 0u; i < lenTitleBlocks; ++i)
    {
        titleBlocks.push_back(dynamic_pointer_cast<StructTitleBlock>(parser.readStructure()));
    }

    const uint16_t lenT0x34s = ds.readUint16();

    mCtx.mLogger.trace("lenT0x34s = {}", lenT0x34s);

    for(size_t i = 0u; i < lenT0x34s; ++i)
    {
        t0x34s.push_back(dynamic_pointer_cast<StructT0x34>(parser.readStructure()));
    }

    const uint16_t lenT0x35s = ds.readUint16();

    mCtx.mLogger.trace("lenT0x35s = {}", lenT0x35s);

    for(size_t i = 0u; i < lenT0x35s; ++i)
    {
        t0x35s.push_back(dynamic_pointer_cast<StructT0x35>(parser.readStructure()));
    }

    const uint16_t lenB = ds.readUint16();

    mCtx.mLogger.trace("lenB = {}", lenB);

    for(size_t i = 0; i < lenB; ++i)
    {
        // @todo Add attributes
        const std::string net = ds.readStringLenZeroTerm();
        const uint32_t id = ds.readUint32();

        mCtx.mLogger.trace("net = {}", net);
        mCtx.mLogger.trace("id  = {}", id);
    }

    const uint16_t lenWires = ds.readUint16();

    mCtx.mLogger.trace("lenWires = {}", lenWires);

    for(size_t i = 0u; i < lenWires; ++i)
    {
        wires.push_back(dynamic_pointer_cast<StructWire>(parser.readStructure()));
    }

    const uint16_t lenPartInsts = ds.readUint16();

    mCtx.mLogger.trace("lenPartInsts = {}", lenPartInsts);

    for(size_t i = 0u; i < lenPartInsts; ++i)
    {
        partInsts.push_back(dynamic_pointer_cast<StructPartInst>(parser.readStructure()));
    }

    const uint16_t lenPorts = ds.readUint16();

    mCtx.mLogger.trace("lenPorts = {}", lenPorts);

    for(size_t i = 0u; i < lenPorts; ++i)
    {
        ports.push_back(dynamic_pointer_cast<StructPort>(parser.readStructure()));
    }

    const uint16_t lenGlobals = ds.readUint16();

    mCtx.mLogger.trace("lenGlobals = {}", lenGlobals);

    for(size_t i = 0u; i < lenGlobals; ++i)
    {
        globals.push_back(dynamic_pointer_cast<StructGlobal>(parser.readStructure()));

        ds.printUnknownData(5, fmt::format("{}: 0", getMethodName(this, __func__)));
    }

    const uint16_t lenOffPageConnectors = ds.readUint16();

    mCtx.mLogger.trace("lenOffPageConnectors = {}", lenOffPageConnectors);

    for(size_t i = 0u; i < lenOffPageConnectors; ++i)
    {
        offPageConnectors.push_back(dynamic_pointer_cast<StructOffPageConnector>(parser.readStructure()));

        ds.printUnknownData(5, fmt::format("{}: 1", getMethodName(this, __func__)));
    }

    const uint16_t lenERCSymbolInsts = ds.readUint16();

    mCtx.mLogger.trace("lenERCSymbolInsts = {}", lenERCSymbolInsts);

    for(size_t i = 0u; i < lenERCSymbolInsts; ++i)
    {
        ercSymbolInsts.push_back(dynamic_pointer_cast<StructERCSymbolInst>(parser.readStructure()));
    }

    const uint16_t lenBusEntries = ds.readUint16();

    mCtx.mLogger.trace("lenBusEntries = {}", lenBusEntries);

    for(size_t i = 0u; i < lenBusEntries; ++i)
    {
        busEntries.push_back(dynamic_pointer_cast<StructBusEntry>(parser.readStructure()));
    }

    const uint16_t lenGraphicInsts = ds.readUint16();

    mCtx.mLogger.trace("lenGraphicInsts = {}", lenGraphicInsts);

    for(size_t i = 0u; i < lenGraphicInsts; ++i)
    {
        graphicInsts.push_back(dynamic_pointer_cast<StructGraphicInst>(parser.readStructure()));
    }

    const uint16_t len10 = ds.readUint16();

    mCtx.mLogger.trace("len10 = {}", len10);

    for(size_t i = 0u; i < len10; ++i)
    {
        // @todo push structure
        const auto s = parser.readStructure();
        if(s)
        {
            mCtx.mLogger.trace("VERIFYING Page Structure10 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    const uint16_t len11 = ds.readUint16();

    mCtx.mLogger.trace("len11 = {}", len11);

    for(size_t i = 0u; i < len11; ++i)
    {
        // @todo push structure
        const auto s = parser.readStructure();
        if(s)
        {
            mCtx.mLogger.trace("VERIFYING Page Structure11 is {}", NAMEOF_TYPE_RTTI(*s));
        }
    }

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}