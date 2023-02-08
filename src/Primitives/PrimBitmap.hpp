#ifndef PRIMBITMAP_HPP
#define PRIMBITMAP_HPP


#include <cstdint>
#include <filesystem>
#include <ostream>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>

#include "General.hpp"
#include "Primitives/PrimBase.hpp"


namespace fs = std::filesystem;


class PrimBitmap : public PrimBase
{
public:

    PrimBitmap(ParserContext& aCtx) : PrimBase{aCtx}, locX{0}, locY{0},
        x1{0}, y1{0}, x2{0}, y2{0}, bmpWidth{0}, bmpHeight{0}
    { }

    std::string to_string() const override;

    void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) override;

    virtual void accept(Visitor& aVisitor) const override
    {
        aVisitor.visit(*this);
    }

    fs::path writeImgToFile(fs::path aFilePath) const;

    int32_t locX;
    int32_t locY;

    int32_t x1;
    int32_t y1;
    int32_t x2;
    int32_t y2;

    uint32_t bmpWidth;
    uint32_t bmpHeight;

    // @note Looks like the XSD uses Base64 encoding for the data, but I was not
    //       able to extract the correct content from there. Are there some parameters
    //       to adjust in the decoding? https://cryptii.com/pipes/base64-to-binary
    std::vector<uint8_t> rawImgData; // @todo called val in the XSD file
};


[[maybe_unused]]
static std::string to_string(const PrimBitmap& aObj)
{
    std::string str;

    str += fmt::format("{}:\n", nameof::nameof_type<decltype(aObj)>());
    str += fmt::format("{}locX = {}\n", indent(1), aObj.locX);
    str += fmt::format("{}locY = {}\n", indent(1), aObj.locY);
    str += fmt::format("{}x1   = {}\n", indent(1), aObj.x1);
    str += fmt::format("{}y1   = {}\n", indent(1), aObj.y1);
    str += fmt::format("{}x2   = {}\n", indent(1), aObj.x2);
    str += fmt::format("{}y2   = {}\n", indent(1), aObj.y2);
    str += fmt::format("{}bmpWidth  = {}\n", indent(1), aObj.bmpWidth);
    str += fmt::format("{}bmpHeight = {}\n", indent(1), aObj.bmpHeight);

    // @todo Should we print rawImgData somehow? As ASCII image?

    return str;
}


inline std::string PrimBitmap::to_string() const
{
    return ::to_string(*this);
}


[[maybe_unused]]
static std::ostream& operator<<(std::ostream& aOs, const PrimBitmap& aVal)
{
    aOs << to_string(aVal);

    return aOs;
}


#endif // PRIMBITMAP_HPP