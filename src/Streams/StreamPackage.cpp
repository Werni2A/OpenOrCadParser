#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamPackage.hpp"

void StreamPackage::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    const uint16_t lenPartCells = ds.readUint16();

    mCtx.mLogger.trace("lenPartCells = {}", lenPartCells);

    for(size_t i = 0u; i < lenPartCells; ++i)
    {
        partCells.push_back(dynamic_pointer_cast<StructPartCell>(parser.readStructure()));

        const uint16_t lenLibraryParts = ds.readUint16();

        mCtx.mLogger.trace("lenLibraryParts = {}", lenLibraryParts);

        for(size_t i = 0u; i < lenLibraryParts; ++i)
        {
            libraryParts.push_back(dynamic_pointer_cast<StructLibraryPart>(parser.readStructure()));
        }
    }

    package = dynamic_pointer_cast<StructPackage>(parser.readStructure());

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}