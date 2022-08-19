#include <algorithm>
#include <any>
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "../Parser.hpp"
#include "../Structures/DirectoryStruct.hpp"


// @todo check whether this works for all directory files or just a few
DirectoryStruct Parser::parseDirectory()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    DirectoryStruct obj;

    obj.lastModifiedDate = static_cast<time_t>(mDs.readUint32());

    const uint16_t size = mDs.readUint16();

    for(size_t i = 0u; i < size; ++i)
    {
        DirItemType dirItemType;

        dirItemType.name = mDs.readStringLenZeroTerm();

        dirItemType.componentType = ToComponentType(mDs.readUint16());

        // @todo This changes with the version of the file format, so maybe it contains
        //       more details for the format? Or some hash of the specified stream?
        mDs.printUnknownData(14, fmt::format("dirItemType[{:>3}] - 0", i));

        // @todo Just a guess that this is the version but's highly likely
        dirItemType.fileFormatVersion = mDs.readUint16();

        spdlog::debug("fileFormatVersion = {}", dirItemType.fileFormatVersion);

        // Known versions that I obsereved in different files
        // 471 in 17.4-2019 S012 (3898062) [10/18/202]
        // 472 in 17.4-2019 S019 (3959056) [7/8/2021]
        std::vector<uint16_t> knownFileVersions{
                                         445, 446, 447, 448, 449,
                450, 451, 452, 453, 454, 455, 456, 457, 458, 459,
                460, 461, 462, 463, 464, 465, 466, 467, 468, 469,
                470, 471, 472
            };

        if(!std::any_of(knownFileVersions.begin(), knownFileVersions.end(), [&](unsigned version){ return version == dirItemType.fileFormatVersion; }))
        {
            spdlog::critical("Unexpected File Version {}", dirItemType.fileFormatVersion);
        }

        dirItemType.timezone = mDs.readInt16();

        mDs.printUnknownData(2, fmt::format("dirItemType[{:>3}] - 1", i));

        obj.items.push_back(dirItemType);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}