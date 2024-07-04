#ifndef STREAMTYPE_HPP
#define STREAMTYPE_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"

namespace OOCP
{
// @todo Incomplete list
enum class StreamType
{
    AdminData,
    BOMDataStream,
    BundleMapData,
    Cache,
    Cell,
    CellsDirectory,
    DsnStream,
    DTypeD, // $Type$
    ERC,
    ExportBlock,
    ExportBlocksDirectory,
    Graphic,
    GraphicsDirectory,
    Hierarchy,
    HSObjects,
    Library,
    Package,
    PackagesDirectory,
    Page,
    Part,
    PartsDirectory,
    Schematic,
    Symbol,
    SymbolsDirectory,
    ViewsDirectory
};

[[maybe_unused]] static std::string to_string(const StreamType& aVal)
{
    return std::string{magic_enum::enum_name<decltype(aVal)>(aVal)};
}

[[maybe_unused]] static std::ostream& operator<<(std::ostream& aOs, const StreamType& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}
} // namespace OOCP
#endif // STREAMTYPE_HPP
