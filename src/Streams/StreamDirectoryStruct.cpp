#include <algorithm>
#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "Streams/StreamDirectoryStruct.hpp"

void OOCP::StreamDirectoryStruct::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    lastModifiedDate = static_cast<time_t>(ds.readUint32());
    mCtx.mLogger.trace("lastModifiedDate = {}", DateTimeToStr(lastModifiedDate));

    const uint16_t size = ds.readUint16();
    mCtx.mLogger.trace("size = {}", size);

    for(size_t i = 0u; i < size; ++i)
    {
        DirItemType item;

        item.name = ds.readStringLenZeroTerm();

        mCtx.mLogger.trace("name = {}", item.name);

        item.componentType = ToComponentType(ds.readUint16());

        mCtx.mLogger.trace("componentType = {}", OOCP::to_string(item.componentType));

        // @todo This changes with the version of the file format, so maybe it contains
        //       more details for the format? Or some hash of the specified stream?
        ds.printUnknownData(14, fmt::format("item[{:>3}] - 0", i));

        // @todo Just a guess that this is the version but it's highly likely
        item.fileFormatVersion = ds.readUint16();

        mCtx.mLogger.trace("fileFormatVersion = {}", item.fileFormatVersion);

        // Known versions that I observed in different files
        // 471 in 17.4-2019 S012 (3898062) [10/18/202]
        // 472 in 17.4-2019 S019 (3959056) [7/8/2021]
        std::vector<uint16_t> knownFileVersions{445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458,
            459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472};

        if(!std::any_of(knownFileVersions.begin(), knownFileVersions.end(),
               [&](unsigned version) { return version == item.fileFormatVersion; }))
        {
            mCtx.mLogger.warn("Unexpected File Version {}", item.fileFormatVersion);
        }

        item.timezone = ds.readInt16();

        mCtx.mLogger.trace("timezone = {}", item.timezone);

        ds.printUnknownData(2, fmt::format("item[{:>3}] - 1", i));

        items.push_back(item);
    }

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}