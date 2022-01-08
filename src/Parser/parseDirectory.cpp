#include <algorithm>
#include <any>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>


#include "../Parser.hpp"
#include "../Structures/DirectoryStruct.hpp"


// @todo check whether this works for all directory files or just a few
DirectoryStruct Parser::parseDirectory()
{
    DirectoryStruct directoryStruct;

    directoryStruct.lastModifiedDate = static_cast<time_t>(mDs.readUint32());

    const uint16_t size = mDs.readUint16();

    for(size_t i = 0u; i < size; ++i)
    {
        DirItemType dirItemType;

        dirItemType.name = mDs.readStringLenZeroTerm();

        dirItemType.componentType = ToComponentType(mDs.readUint16());

        // @todo This changes with the version of the file format, so maybe it contains
        //       more details for the format? Or some hash of the specified stream?
        mDs.printUnknownData(std::clog, 14, "item " + std::to_string(i));

        // @todo Just a guess that this is the version but's highly likely
        uint16_t probFileVersion = mDs.readUint16();
        std::cout << "FileVersion?? = " << std::to_string(probFileVersion) << std::endl;

        if(storedVersion == 0)
        {
            storedVersion = probFileVersion;
        }
        else
        {
            if(static_cast<int>(storedVersion / 10) != static_cast<int>(probFileVersion / 10))
            {
                std::cout << "FileVersion Difference " << std::to_string(storedVersion) << " != " << std::to_string(probFileVersion) << std::endl;
            }
        }

        // 471 in 17.4-2019 S012 (3898062) [10/18/202]
        // 472 in 17.4-2019 S019 (3959056) [7/8/2021]
        std::vector<unsigned> knownFileVersions{445, 458, 459, 460, 461, 465, 466, 467, 468, 470, 471, 472};

        if(!std::any_of(knownFileVersions.begin(), knownFileVersions.end(), [&](unsigned version){ return version == probFileVersion; }))
        {
            throw std::runtime_error("Unexpected File Version " + std::to_string(probFileVersion));
        }

        dirItemType.timezone = mDs.readInt16();

        mDs.printUnknownData(std::clog, 2, "item " + std::to_string(i));

        directoryStruct.items.push_back(dirItemType);
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    std::clog << directoryStruct << std::endl;

    return directoryStruct;
}