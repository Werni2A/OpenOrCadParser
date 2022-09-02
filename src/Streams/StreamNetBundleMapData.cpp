#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Parser.hpp"
#include "Streams/StreamNetBundleMapData.hpp"


StreamNetBundleMapData Parser::readStreamNetBundleMapData()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    StreamNetBundleMapData obj;

    mDs.discardBytes(2);

    uint16_t number_groups = mDs.readUint16();

    spdlog::debug("number_groups = {}", number_groups);

    for(size_t i = 0U; i < number_groups; ++i)
    {
        spdlog::debug("[{}]:", i);

        std::string group_name = mDs.readStringLenZeroTerm();
        spdlog::debug("group_name = {}:", group_name);

        // Structure structure = read_prefixes(3);
        Structure structure = auto_read_prefixes();

        if(static_cast<int>(structure) != 0x67)
        {
            throw std::runtime_error(fmt::format("Expected 0x67 but got {}", static_cast<int>(structure)));
        }

        readPreamble();

        mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 0");

        // The following contains the information of a net group but
        // the name of this group is stated outside. How should this
        // be handled?

        uint16_t number_group_elements = mDs.readUint16();

        for(size_t j = 0U; j < number_group_elements; ++j)
        {
            std::string element_name = mDs.readStringLenZeroTerm();
            spdlog::debug("  [{}]: element_name = {}", j, element_name);

            // @todo 0x01 is probably a scalar wire
            //       0x02 is probably a bus
            uint16_t wire_type = mDs.readUint16();
            spdlog::debug("       wire_type = {}", wire_type == 0x01 ? "Scalar" : wire_type == 0x02 ? "Bus" : "Unknown");
        }
    }

    // @todo use functino
    if(!mDs.isEoF())
    {
        throw std::runtime_error("Exptected EoF in NetBundleMapData");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}