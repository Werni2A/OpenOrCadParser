#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "Streams/StreamNetBundleMapData.hpp"


void StreamNetBundleMapData::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    mDs.get().printUnknownData(2, fmt::format("{}: 0", __func__));

    uint16_t number_groups = mDs.get().readUint16();

    spdlog::trace("number_groups = {}", number_groups);

    for(size_t i = 0U; i < number_groups; ++i)
    {
        spdlog::trace("[{}]:", i);

        std::string group_name = mDs.get().readStringLenZeroTerm();
        spdlog::trace("group_name = {}:", group_name);

        Structure structure = auto_read_prefixes();

        // @todo extract the following into a separate readStructNetGroup method
        if(structure != Structure::NetGroup)
        {
            const std::string msg = fmt::format("{}: Expected {} but got {}",
                __func__, ::to_string(Structure::NetGroup), ::to_string(structure));

            spdlog::error(msg);
            throw std::runtime_error(msg);
        }

        readPreamble();

        mDs.get().assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

        // The following contains the information of a net group but
        // the name of this group is stated outside. How should this
        // be handled?

        uint16_t number_group_elements = mDs.get().readUint16();

        for(size_t j = 0U; j < number_group_elements; ++j)
        {
            std::string element_name = mDs.get().readStringLenZeroTerm();
            spdlog::trace("  [{}]: element_name = {}", j, element_name);

            // @todo 0x01 is probably a scalar wire
            //       0x02 is probably a bus
            uint16_t wire_type = mDs.get().readUint16();
            spdlog::trace("       wire_type = {}", wire_type == 0x01 ? "Scalar" : wire_type == 0x02 ? "Bus" : "Unknown");
        }
    }

    if(!mDs.get().isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}