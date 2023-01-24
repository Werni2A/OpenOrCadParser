#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "PageSettings.hpp"
#include "Streams/StreamPage.hpp"


void StreamPage::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    name = mDs.get().readStringLenZeroTerm();

    spdlog::trace("name = {}", name);

    pageSize = mDs.get().readStringLenZeroTerm();

    spdlog::trace("pageSize = {}", pageSize);

    pageSettings.read();

    // @todo Contains StructTitleBlock
    const uint16_t lenA = mDs.get().readUint16();

    spdlog::trace("lenA = {}", lenA);

    for(size_t i = 0u; i < lenA; ++i)
    {
        spdlog::critical("VERIFYING Page StructureA is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    const uint16_t lenT0x34s = mDs.get().readUint16();

    spdlog::trace("lenT0x34s = {}", lenT0x34s);

    for(size_t i = 0u; i < lenT0x34s; ++i)
    {
        t0x34s.push_back(dynamic_pointer_cast<StructT0x34>(readStructure()));
    }

    const uint16_t lenT0x35s = mDs.get().readUint16();

    spdlog::trace("lenT0x35s = {}", lenT0x35s);

    for(size_t i = 0u; i < lenT0x35s; ++i)
    {
        t0x35s.push_back(dynamic_pointer_cast<StructT0x35>(readStructure()));
    }

    const uint16_t lenB = mDs.get().readUint16();

    spdlog::trace("lenB = {}", lenB);

    for(size_t i = 0; i < lenB; ++i)
    {
        const std::string net = mDs.get().readStringLenZeroTerm();
        const uint32_t id = mDs.get().readUint32();

        spdlog::trace("net = {}", net);
        spdlog::trace("id  = {}", id);
    }

    const uint16_t lenWires = mDs.get().readUint16();

    spdlog::trace("lenWires = {}", lenWires);

    for(size_t i = 0u; i < lenWires; ++i)
    {
        wires.push_back(dynamic_pointer_cast<StructWire>(readStructure()));
    }

    const uint16_t lenPartInsts = mDs.get().readUint16();

    spdlog::trace("lenPartInsts = {}", lenPartInsts);

    for(size_t i = 0u; i < lenPartInsts; ++i)
    {
        partInsts.push_back(dynamic_pointer_cast<StructPartInst>(readStructure()));
    }

    const uint16_t lenPorts = mDs.get().readUint16();

    spdlog::trace("lenPorts = {}", lenPorts);

    for(size_t i = 0u; i < lenPorts; ++i)
    {
        ports.push_back(dynamic_pointer_cast<StructPort>(readStructure()));
    }

    const uint16_t len5 = mDs.get().readUint16();

    spdlog::trace("len5 = {}", len5);

    for(size_t i = 0u; i < len5; ++i)
    {
        spdlog::critical("VERIFYING Page Structure5 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
        mDs.get().printUnknownData(5, fmt::format("{}: 0", __func__));
    }

    const uint16_t len6 = mDs.get().readUint16();

    spdlog::trace("len6 = {}", len6);

    for(size_t i = 0u; i < len6; ++i)
    {
        spdlog::critical("VERIFYING Page Structure6 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    const uint16_t len7 = mDs.get().readUint16();

    spdlog::trace("len7 = {}", len7);

    for(size_t i = 0u; i < len7; ++i)
    {
        spdlog::critical("VERIFYING Page Structure7 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    const uint16_t len8 = mDs.get().readUint16();

    spdlog::trace("len8 = {}", len8);

    for(size_t i = 0u; i < len8; ++i)
    {
        spdlog::critical("VERIFYING Page Structure8 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    const uint16_t lenGraphicInsts = mDs.get().readUint16();

    spdlog::trace("lenGraphicInsts = {}", lenGraphicInsts);

    // @todo Create class GraphicInst and derive all the Graphic*Inst
    //       classes from it
    for(size_t i = 0u; i < lenGraphicInsts; ++i)
    {
        spdlog::critical("VERIFYING Page Structure9 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    const uint16_t len10 = mDs.get().readUint16();

    spdlog::trace("len10 = {}", len10);

    for(size_t i = 0u; i < len10; ++i)
    {
        spdlog::critical("VERIFYING Page Structure10 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    const uint16_t len11 = mDs.get().readUint16();

    spdlog::trace("len11 = {}", len11);

    for(size_t i = 0u; i < len11; ++i)
    {
        spdlog::critical("VERIFYING Page Structure11 is {}", NAMEOF_TYPE_RTTI(*readStructure().get())); // @todo push structure
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}