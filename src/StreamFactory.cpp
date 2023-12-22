#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "CommonBase.hpp"
#include "ParserContext.hpp"
#include "StreamFactory.hpp"
#include "Streams/StreamAdminData.hpp"
#include "Streams/StreamCache.hpp"
#include "Streams/StreamCellsDirectory.hpp"
#include "Streams/StreamDsnStream.hpp"
#include "Streams/StreamExportBlocksDirectory.hpp"
#include "Streams/StreamGraphicsDirectory.hpp"
#include "Streams/StreamHierarchy.hpp"
#include "Streams/StreamHSObjects.hpp"
#include "Streams/StreamLibrary.hpp"
#include "Streams/StreamNetBundleMapData.hpp"
#include "Streams/StreamPackage.hpp"
#include "Streams/StreamPackagesDirectory.hpp"
#include "Streams/StreamPage.hpp"
#include "Streams/StreamPartsDirectory.hpp"
#include "Streams/StreamSchematic.hpp"
#include "Streams/StreamSymbol.hpp"
#include "Streams/StreamSymbolsDirectory.hpp"
#include "Streams/StreamType.hpp"
#include "Streams/StreamViewsDirectory.hpp"


std::unique_ptr<CommonBase> StreamFactory::build(ParserContext& aCtx, const std::vector<std::string>& aCfbfStreamLocation)
{
    const auto& loc = aCfbfStreamLocation;

    std::string locStr{};
    for(const auto& locPart : loc)
    {
        locStr += "/" + locPart;
    }

    spdlog::debug("Got stream location: {}", locStr);

    // Match `AdminData`
    if(loc.size() == 1U && loc.at(0U) == "AdminData")
    {
        return std::make_unique<StreamAdminData>(aCtx);
    }

    // Match `Cache`
    if(loc.size() == 1U && loc.at(0U) == "Cache")
    {
        return std::make_unique<StreamCache>(aCtx);
    }

    // Match `Cells/*`
    if(loc.size() == 2U && loc.at(0U) == "Cells")
    {
        // return std::make_unique<StreamCells>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected Cell but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Cells Directory`
    if(loc.size() == 1U && loc.at(0U) == "Cells Directory")
    {
        return std::make_unique<StreamCellsDirectory>(aCtx);
    }

    // Match `CIS/CISSchematicStore/CISSchematicStream`
    if(loc.size() == 3U && loc.at(0U) == "CIS"
        && loc.at(1U) == "CISSchematicStore"
        && loc.at(2U) == "CISSchematicStream")
    {
        // return std::make_unique<StreamCISSchematicStream>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected CISSchematicStream but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `CIS/VariantStore/BOM/BOMDataStream`
    if(loc.size() == 4U && loc.at(0U) == "CIS"
        && loc.at(1U) == "VariantStore"
        && loc.at(2U) == "BOM"
        && loc.at(3U) == "BOMDataStream")
    {
        // return std::make_unique<BOMDataStream>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected BOMDataStream but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `CIS/VariantStore/Groups/GroupsDataStream`
    if(loc.size() == 4U && loc.at(0U) == "CIS"
        && loc.at(1U) == "VariantStore"
        && loc.at(2U) == "Groups"
        && loc.at(3U) == "GroupsDataStream")
    {
        // return std::make_unique<GroupsDataStream>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected GroupsDataStream but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `CIS/VariantStore/VariantNames`
    if(loc.size() == 3U && loc.at(0U) == "CIS"
        && loc.at(1U) == "VariantStore"
        && loc.at(2U) == "VariantNames")
    {
        // return std::make_unique<VariantNames>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected VariantName but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `DsnStream`
    if(loc.size() == 1U && loc.at(0U) == "DsnStream")
    {
        return std::make_unique<StreamDsnStream>(aCtx);
    }

    // Match `ExportBlocks/*`
    if(loc.size() == 2U && loc.at(0U) == "ExportBlocks")
    {
        // return std::make_unique<StreamExportBlock>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected ExportBlock but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `ExportBlocks Directory`
    if(loc.size() == 1U && loc.at(0U) == "ExportBlocks Directory")
    {
        return std::make_unique<StreamExportBlocksDirectory>(aCtx);
    }

    // Match `Graphics/$Types$`
    if(loc.size() == 2U && loc.at(0U) == "Graphics"
        && loc.at(1U) == "$Types$")
    {
        return std::make_unique<StreamType>(aCtx);
    }

    // Match `Graphics/*`
    if(loc.size() == 2U && loc.at(0U) == "Graphics"
        && loc.at(1U) != "$Types$")
    {
        // return std::make_unique<StreamGraphics>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected Graphic but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Graphics Directory`
    if(loc.size() == 1U && loc.at(0U) == "Graphics Directory")
    {
        return std::make_unique<StreamGraphicsDirectory>(aCtx);
    }

    // Match `HSObjects`
    if(loc.size() == 1U && loc.at(0U) == "HSObjects")
    {
        return std::make_unique<StreamHSObjects>(aCtx);
    }

    // Match `LayoutReuse/ReuseSchematics`
    if(loc.size() == 2U && loc.at(0U) == "LayoutReuse"
        && loc.at(1U) == "ReuseSchematics")
    {
        // return std::make_unique<StreamReuseSchematic>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected ReuseSchematic but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Library`
    if(loc.size() == 1U && loc.at(0U) == "Library")
    {
        return std::make_unique<StreamLibrary>(aCtx);
    }

    // Match `NetBundleMapData`
    if(loc.size() == 1U && loc.at(0U) == "NetBundleMapData")
    {
        return std::make_unique<StreamNetBundleMapData>(aCtx);
    }

    // Match `Packages/*`
    if(loc.size() == 2U && loc.at(0U) == "Packages")
    {
        // We extract embedded files into the same directory but do
        // not want to parse them as stream. Therefore skip them.
        // @todo This is just a workaround; needs some refactoring
        // if(!fs::path(loc.at(1)).has_extension())
        {
            return std::make_unique<StreamPackage>(aCtx);
        }
    }

    // Match `Packages Directory`
    if(loc.size() == 1U && loc.at(0U) == "Packages Directory")
    {
        return std::make_unique<StreamPackagesDirectory>(aCtx);
    }

    // Match `Parts/*`
    if(loc.size() == 2U && loc.at(0U) == "Parts")
    {
        // return std::make_unique<StreamPart>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected Part but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Parts Directory`
    if(loc.size() == 1U && loc.at(0U) == "Parts Directory")
    {
        return std::make_unique<StreamPartsDirectory>(aCtx);
    }

    // Match `Symbols/$Types$`
    if(loc.size() == 2U && loc.at(0U) == "Symbols"
        && loc.at(1U) == "$Types$")
    {
        // return std::make_unique<StreamType>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected Symbols/$Types$ but stream parser is skipped!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Symbols/ERC`
    if(loc.size() == 2U && loc.at(0U) == "Symbols"
        && loc.at(1U) == "ERC")
    {
        // return std::make_unique<StreamERC>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected Symbols/ERC but stream parser is skipped!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Symbols/ERC_PHYSICAL`
    if(loc.size() == 2U && loc.at(0U) == "Symbols"
        && loc.at(1U) == "ERC_PHYSICAL")
    {
        // return std::make_unique<StreamERCPHYSICAL>(aCtx);
        const std::string errMsg = fmt::format(
            "Detected ERC_PHYSICAL but stream parser is not yet implemented!");
        spdlog::warn(errMsg);

        return std::unique_ptr<CommonBase>{};
    }

    // Match `Symbols/*`
    if(loc.size() == 2U && loc.at(0U) == "Symbols"
        && loc.at(1U) != "$Types$"
        && loc.at(1U) != "ERC"
        && loc.at(1U) != "ERC_PHYSICAL")
    {
        // We extract embedded files into the same directory but do
        // not want to parse them as stream. Therefore skip them.
        // @todo This is just a workaround; needs some refactoring
        // if(!fs::path(loc.at(1)).has_extension())
        {
            return std::make_unique<StreamSymbol>(aCtx);
        }
    }

    // Match `Symbols Directory`
    if(loc.size() == 1U && loc.at(0U) == "Symbols Directory")
    {
        return std::make_unique<StreamSymbolsDirectory>(aCtx);
    }

    // Match `Views/*/Hierarchy/Hierarchy`
    if(loc.size() == 4U && loc.at(0U) == "Views"
        && loc.at(2U) == "Hierarchy"
        && loc.at(3U) == "Hierarchy")
    {
        return std::make_unique<StreamHierarchy>(aCtx);
    }

    // Match `Views/*/Pages/*`
    if(loc.size() == 4U && loc.at(0U) == "Views"
        && loc.at(2U) == "Pages")
    {
        return std::make_unique<StreamPage>(aCtx);
    }

    // Match `Views/*/Schematic`
    if(loc.size() == 3U && loc.at(0U) == "Views"
        && loc.at(2U) == "Schematic")
    {
        return std::make_unique<StreamSchematic>(aCtx);
    }

    // Match `Views Directory`
    if(loc.size() == 1U && loc.at(0U) == "Views Directory")
    {
        return std::make_unique<StreamViewsDirectory>(aCtx);
    }

    std::string locPath;
    std::for_each(loc.cbegin(), loc.cend(),
        [&locPath](const std::string& pathPart){ locPath += "/" + pathPart;});

    const std::string errMsg = fmt::format("Didn't find a suitable stream parser for loc = {}", locPath);
    spdlog::warn(errMsg);

    return std::unique_ptr<CommonBase>{};
}