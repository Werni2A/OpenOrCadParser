#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "ContainerContext.hpp"
#include "Stream.hpp"
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


std::unique_ptr<Stream> StreamFactory::build(ContainerContext& aCtx, const fs::path& aInputStream)
{
    auto streamLoc = CfbfStreamLocation{aInputStream, aCtx.mExtractedCfbfPath};

    aCtx.mLogger.debug("Got stream location: {}", ::to_string(streamLoc));

    const auto getErrMsg = [](const std::vector<std::optional<std::string>>& aPattern, const CfbfStreamLocation& aStreamLoc)
        {
            std::string strPattern{};

            for(const auto& part : aPattern)
            {
                strPattern += "/" + part.value_or("*");
            }

            return fmt::format(
                "Detected `{}` matching `{}` but stream parser is not yet implemented!",
                ::to_string(aStreamLoc), strPattern
                );
        };

    std::vector<std::optional<std::string>> pattern;

    // Match `/AdminData`
    pattern = {"AdminData"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamAdminData>(aCtx, aInputStream);
    }

    // Match `/BundleMapData`
    pattern = {"BundleMapData"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Cache`
    pattern = {"Cache"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamCache>(aCtx, aInputStream);
    }

    // Match `/Cells/*`
    pattern = {"Cells", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Cells Directory`
    pattern = {"Cells Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamCellsDirectory>(aCtx, aInputStream);
    }

    // Match `/CIS/CISSchematicStore/CISSchematicStream`
    pattern = {"CIS", "CISSchematicStore", "CISSchematicStream"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/BOM/BOMDataStream`
    pattern = {"CIS", "VariantStore", "BOM", "BOMDataStream"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/BOM/*/BOMAmbugity`
    pattern = {"CIS", "VariantStore", "BOM", std::nullopt, "BOMAmbugity"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/BOM/*/BOMPartData`
    pattern = {"CIS", "VariantStore", "BOM", std::nullopt, "BOMPartData"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/BOM/*/*`
    pattern = {"CIS", "VariantStore", "BOM", std::nullopt, std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/Groups/GroupsDataStream`
    pattern = {"CIS", "VariantStore", "Groups", "GroupsDataStream"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/Groups/*/*`
    pattern = {"CIS", "VariantStore", "Groups", std::nullopt, std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/CIS/VariantStore/VariantNames`
    pattern = {"CIS", "VariantStore", "VariantNames"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/DsnStream`
    pattern = {"DsnStream"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamDsnStream>(aCtx, aInputStream);
    }

    // Match `/ExportBlocks/*`
    pattern = {"ExportBlocks", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/ExportBlocks Directory`
    pattern = {"ExportBlocks Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamExportBlocksDirectory>(aCtx, aInputStream);
    }

    // Match `/Graphics/$Types$`
    pattern = {"Graphics", "$Types$"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamType>(aCtx, aInputStream);
    }

    // Match `/Graphics/*`
    pattern = {"Graphics", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Graphics Directory`
    pattern = {"Graphics Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamGraphicsDirectory>(aCtx, aInputStream);
    }

    // Match `/HSObjects`
    pattern = {"HSObjects"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamHSObjects>(aCtx, aInputStream);
    }

    // Match `/LayoutReuse/ReuseSchematics`
    pattern = {"LayoutReuse", "ReuseSchematics"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Library`
    pattern = {"Library"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamLibrary>(aCtx, aInputStream);
    }

    // Match `/NetBundleMapData`
    pattern = {"NetBundleMapData"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamNetBundleMapData>(aCtx, aInputStream);
    }

    // Match `/Packages/*`
    pattern = {"Packages", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamPackage>(aCtx, aInputStream);
    }

    // Match `/Packages Directory`
    pattern = {"Packages Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamPackagesDirectory>(aCtx, aInputStream);
    }

    // Match `/Parts/*`
    pattern = {"Parts", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Parts Directory`
    pattern = {"Parts Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamPartsDirectory>(aCtx, aInputStream);
    }

    // Match `/Symbols/$Types$`
    pattern = {"Symbols", "$Types$"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Symbols/ERC`
    pattern = {"Symbols", "ERC"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Symbols/ERC_PHYSICAL`
    pattern = {"Symbols", "ERC_PHYSICAL"};
    if(streamLoc.matches_pattern(pattern))
    {
        aCtx.mLogger.warn(getErrMsg(pattern, streamLoc));
        return std::unique_ptr<Stream>{};
    }

    // Match `/Symbols/*`
    pattern = {"Symbols", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamSymbol>(aCtx, aInputStream);
    }

    // Match `/Symbols Directory`
    pattern = {"Symbols Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamSymbolsDirectory>(aCtx, aInputStream);
    }

    // Match `/Views/*/Hierarchy/Hierarchy`
    pattern = {"Views", std::nullopt, "Hierarchy", "Hierarchy"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamHierarchy>(aCtx, aInputStream);
    }

    // Match `/Views/*/Pages/*`
    pattern = {"Views", std::nullopt, "Pages", std::nullopt};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamPage>(aCtx, aInputStream);
    }

    // Match `/Views/*/Schematic`
    pattern = {"Views", std::nullopt, "Schematic"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamSchematic>(aCtx, aInputStream);
    }

    // Match `/Views Directory`
    pattern = {"Views Directory"};
    if(streamLoc.matches_pattern(pattern))
    {
        return std::make_unique<StreamViewsDirectory>(aCtx, aInputStream);
    }

    const std::string errMsg = fmt::format(
        "Didn't find a suitable stream parser for stream location `{}`",
        ::to_string(streamLoc)
        );

    aCtx.mLogger.warn(errMsg);

    return std::unique_ptr<Stream>{};
}