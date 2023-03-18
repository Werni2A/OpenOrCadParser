#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Streams/StreamNetBundleMapData.hpp"


void StreamNetBundleMapData::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.get().mDs.get();

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ds.printUnknownData(2, fmt::format("{}: 0", getMethodName(this, __func__)));

    uint16_t number_groups = ds.readUint16();

    spdlog::trace("number_groups = {}", number_groups);

    for(size_t i = 0U; i < number_groups; ++i)
    {
        spdlog::trace("[{}]:", i);

        std::string group_name = ds.readStringLenZeroTerm();
        spdlog::trace("group_name = {}:", group_name);

        // ----------------------------------------

        FutureDataLst localFutureLst{mCtx};

        auto_read_prefixes(Structure::NetGroup, localFutureLst);

        readPreamble();

        localFutureLst.checkpoint();

        ds.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

        localFutureLst.checkpoint();

        localFutureLst.sanitizeCheckpoints();

        // ----------------------------------

        uint16_t number_group_elements = ds.readUint16();

        for(size_t j = 0U; j < number_group_elements; ++j)
        {
            std::string element_name = ds.readStringLenZeroTerm();
            spdlog::trace("  [{}]: element_name = {}", j, element_name);

            // @todo 0x01 is probably a scalar wire
            //       0x02 is probably a bus
            uint16_t wire_type = ds.readUint16();
            spdlog::trace("       wire_type = {}", wire_type == 0x01 ? "Scalar" : wire_type == 0x02 ? "Bus" : "Unknown");
        }
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}