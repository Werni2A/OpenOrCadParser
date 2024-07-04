#ifndef CFBFSTREAMLOCATION_HPP
#define CFBFSTREAMLOCATION_HPP

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "General.hpp"

namespace fs = std::filesystem;

namespace OOCP
{

class CfbfStreamLocation
{
public:
    CfbfStreamLocation(const fs::path& aStreamPath, const fs::path& aExtractedCfbfPath)
    {
        std::vector<std::string> streamLocation{};

        // The layout in the filesystem is equal
        // to the one inside the CFBF container
        const fs::path relPathStreamLoc = fs::relative(aStreamPath, aExtractedCfbfPath);

        for(const auto& pathPart : relPathStreamLoc)
        {
            streamLocation.push_back(pathPart.string());
        }

        if(!streamLocation.empty())
        {
            // Remove file extension (`.bin`)
            streamLocation.back() = fs::path{streamLocation.back()}.replace_extension().string();
        }

        mStreamLocation = std::move(streamLocation);
    }

    fs::path get_relative_fs_path() const
    {
        const auto vec = get_vector();

        fs::path fsPath{};

        for(const auto& part : vec)
        {
            fsPath = fsPath / part;
        }

        return fsPath;
    }

    const std::vector<std::string>& get_vector() const
    {
        return mStreamLocation;
    }

    /**
     * Check if the CfbfStreamLocation matches the given pattern. Note
     * that the vector size needs to be equal and all given string parts
     * need to match. As a wildcard part, you can use std::nullopt
     */
    bool matches_pattern(const std::vector<std::optional<std::string>>& aPattern)
    {
        if(get_vector().size() != aPattern.size())
        {
            return false;
        }

        // Both vectors have the same length
        for(std::size_t i{0U}; i < get_vector().size(); ++i)
        {
            // Check for wildcard and skip if found
            if(!aPattern.at(i).has_value())
            {
                continue;
            }

            // Check that the location part is equal
            if(get_vector().at(i) != aPattern.at(i).value())
            {
                return false;
            }
        }

        return true;
    }

private:
    std::vector<std::string> mStreamLocation{};
};

[[maybe_unused]] static std::string to_string(const CfbfStreamLocation& aLocation)
{
    std::string locStr{};
    for(const auto& locPart : aLocation.get_vector())
    {
        locStr += "/" + locPart;
    }

    return locStr;
}

[[maybe_unused]] static bool operator==(const CfbfStreamLocation& aLhs, const std::vector<std::string>& aRhs)
{
    return aLhs.get_vector() == aRhs;
}

[[maybe_unused]] static bool operator==(const std::vector<std::string>& aLhs, const CfbfStreamLocation& aRhs)
{
    return aRhs == aLhs;
}

[[maybe_unused]] static bool operator==(const CfbfStreamLocation& aLhs, const CfbfStreamLocation& aRhs)
{
    return aLhs.get_vector() == aRhs.get_vector();
}
} // namespace OOCP
#endif // CFBFSTREAMLOCATION_HPP