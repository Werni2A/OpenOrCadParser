#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamNetBundleMapData.hpp"

void OOCP::StreamNetBundleMapData::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    ds.printUnknownData(2, fmt::format("{}: 0", getMethodName(this, __func__)));

    uint16_t number_groups = ds.readUint16();

    mCtx.mLogger.trace("number_groups = {}", number_groups);

    for(size_t i = 0U; i < number_groups; ++i)
    {
        mCtx.mLogger.trace("[{}]:", i);

        std::string group_name = ds.readStringLenZeroTerm();
        mCtx.mLogger.trace("group_name = {}:", group_name);

        // ----------------------------------------

        FutureDataLst localFutureLst{mCtx};

        parser.auto_read_prefixes(Structure::NetGroup, localFutureLst);

        parser.readPreamble();

        localFutureLst.checkpoint();

        ds.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, getMethodName(this, __func__) + ": 0");

        localFutureLst.checkpoint();

        localFutureLst.sanitizeCheckpoints();

        // ----------------------------------

        uint16_t number_group_elements = ds.readUint16();

        for(size_t j = 0U; j < number_group_elements; ++j)
        {
            std::string element_name = ds.readStringLenZeroTerm();
            mCtx.mLogger.trace("  [{}]: element_name = {}", j, element_name);

            // @todo 0x01 is probably a scalar wire
            //       0x02 is probably a bus
            uint16_t wire_type = ds.readUint16();
            mCtx.mLogger.trace("       wire_type = {}", wire_type == 0x01   ? "Scalar"
                                                        : wire_type == 0x02 ? "Bus"
                                                                            : "Unknown");
        }
    }

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}