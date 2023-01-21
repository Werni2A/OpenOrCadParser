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

    spdlog::debug("name = {}", name);

    pageSize = mDs.get().readStringLenZeroTerm();

    spdlog::debug("pageSize = {}", pageSize);

    pageSettings.read();

    // Probably Title Blocks
    const uint16_t lenA = mDs.get().readUint16();

    spdlog::debug("lenA = {}", lenA);

    for(size_t i = 0u; i < lenA; ++i)
    {
        readStructure(); // @todo push structure
    }

    // Probably Bus
    const uint16_t len0 = mDs.get().readUint16();

    spdlog::debug("len0 = {}", len0);

    for(size_t i = 0u; i < len0; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len1 = mDs.get().readUint16();

    spdlog::debug("len1 = {}", len1);

    for(size_t i = 0u; i < len1; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t lenB = mDs.get().readUint16();

    spdlog::debug("lenB = {}", lenB);

    for(size_t i = 0; i < lenB; ++i)
    {
        const std::string net = mDs.get().readStringLenZeroTerm();
        const uint32_t id = mDs.get().readUint32();

        spdlog::debug("net = {}", net);
        spdlog::debug("id  = {}", id);
    }

    const uint16_t len2 = mDs.get().readUint16();

    spdlog::debug("len2 = {}", len2);

    for(size_t i = 0u; i < len2; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len3 = mDs.get().readUint16();

    spdlog::debug("len3 = {}", len3);

    for(size_t i = 0u; i < len3; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len4 = mDs.get().readUint16();

    spdlog::debug("len4 = {}", len4);

    for(size_t i = 0u; i < len4; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len5 = mDs.get().readUint16();

    spdlog::debug("len5 = {}", len5);

    for(size_t i = 0u; i < len5; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len6 = mDs.get().readUint16();

    spdlog::debug("len6 = {}", len6);

    for(size_t i = 0u; i < len6; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len7 = mDs.get().readUint16();

    spdlog::debug("len7 = {}", len7);

    for(size_t i = 0u; i < len7; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len8 = mDs.get().readUint16();

    spdlog::debug("len8 = {}", len8);

    for(size_t i = 0u; i < len8; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len9 = mDs.get().readUint16();

    spdlog::debug("len9 = {}", len9);

    for(size_t i = 0u; i < len9; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len10 = mDs.get().readUint16();

    spdlog::debug("len10 = {}", len10);

    for(size_t i = 0u; i < len10; ++i)
    {
        readStructure(); // @todo push structure
    }

    const uint16_t len11 = mDs.get().readUint16();

    spdlog::debug("len11 = {}", len11);

    for(size_t i = 0u; i < len11; ++i)
    {
        readStructure(); // @todo push structure
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}