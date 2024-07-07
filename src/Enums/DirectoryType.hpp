#ifndef DIRECTORYTYPE_HPP
#define DIRECTORYTYPE_HPP

#include <cstdint>
#include <ostream>
#include <string>

#include <magic_enum.hpp>

#include "General.hpp"

namespace OOCP
{
enum class DirectoryType
{
    CellsDirectory,
    ExportBlocksDirectory,
    GraphicsDirectory,
    PackagesDirectory,
    PartsDirectory,
    SymbolsDirectory,
    ViewsDirectory
};

[[maybe_unused]]
static std::string to_string(const DirectoryType& aVal)
{
    switch(aVal)
    {
        case DirectoryType::CellsDirectory:
            return std::string{"Cells Directory"};
            break;
        case DirectoryType::ExportBlocksDirectory:
            return std::string{"ExportBlocks Directory"};
            break;
        case DirectoryType::GraphicsDirectory:
            return std::string{"Graphics Directory"};
            break;
        case DirectoryType::PackagesDirectory:
            return std::string{"Packages Directory"};
            break;
        case DirectoryType::PartsDirectory:
            return std::string{"Parts Directory"};
            break;
        case DirectoryType::SymbolsDirectory:
            return std::string{"Symbols Directory"};
            break;
        case DirectoryType::ViewsDirectory:
            return std::string{"Views Directory"};
            break;
        default:
            throw std::runtime_error{"Unimplemented DirectoryType!"};
    }
}

[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const DirectoryType& aVal)
{
    aOs << to_string(aVal);
    return aOs;
}
} // namespace OOCP
#endif // DIRECTORYTYPE_HPP
