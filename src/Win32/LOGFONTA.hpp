#ifndef LOGFONTA_HPP
#define LOGFONTA_HPP


#include <algorithm>
#include <cstdint>
#include <string>

#include <fmt/core.h>
#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "General.hpp"
#include "StreamContext.hpp"


// See Win32 API for reference, where the original structure was copied from
// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-logfonta
class tagLOGFONTA
{
public:
    tagLOGFONTA() : lfHeight{0}, lfWidth{0}, lfEscapement{0}, lfOrientation{0},
        lfWeight{0}, lfItalic{0}, lfUnderline{0}, lfStrikeOut{0}, lfCharSet{0},
        lfOutPrecision{0}, lfClipPrecision{0}, lfQuality{0}, lfPitchAndFamily{0},
        lfFaceName{""}
    { }

    void read(StreamContext& aCtx)
    {
        spdlog::debug(getOpeningMsg(getMethodName(this, __func__), aCtx.mDs.getCurrentOffset()));

        const auto buffer = aCtx.mDs.readBytes(sizeof(*this));

        std::memcpy(this, buffer.data(), sizeof(*this));

        if(lfWeight != std::clamp(lfWeight, 0, 1000))
        {
            throw std::runtime_error(fmt::format("0 <= lfWeight <= 1000 must hold but lfWeight = {}", lfWeight));
        }

        if(lfOutPrecision > 10U)
        {
            throw std::runtime_error(fmt::format("lfOutPrecision <= 10 must hold but lfOutPrecision = {}", lfOutPrecision));
        }

        if(lfQuality > 6U)
        {
            throw std::runtime_error(fmt::format("lfQuality <= 6 must hold but lfQuality = {}", lfQuality));
        }

        spdlog::debug(getClosingMsg(getMethodName(this, __func__), aCtx.mDs.getCurrentOffset()));
        spdlog::trace(to_string());
    }

    void accept(Visitor& aVisitor) const
    {
        aVisitor.visit(*this);
    }

    std::string to_string() const
    {
        std::string str;

        str += fmt::format("{}:\n", nameof::nameof_type<decltype(*this)>());
        str += fmt::format("{}lfHeight          = {}\n", indent(1), lfHeight);
        str += fmt::format("{}lfWidth           = {}\n", indent(1), lfWidth);
        str += fmt::format("{}lfEscapement      = {}\n", indent(1), lfEscapement);
        str += fmt::format("{}lfOrientation     = {}\n", indent(1), lfOrientation);
        str += fmt::format("{}lfWeight          = {}\n", indent(1), lfWeight);
        str += fmt::format("{}lfItalic          = {}\n", indent(1), lfItalic);
        str += fmt::format("{}lfUnderline       = {}\n", indent(1), lfUnderline);
        str += fmt::format("{}lfStrikeOut       = {}\n", indent(1), lfStrikeOut);
        str += fmt::format("{}lfCharSet         = {}\n", indent(1), lfCharSet);
        str += fmt::format("{}lfOutPrecision    = {}\n", indent(1), lfOutPrecision);
        str += fmt::format("{}lfClipPrecision   = {}\n", indent(1), lfClipPrecision);
        str += fmt::format("{}lfQuality         = {}\n", indent(1), lfQuality);
        str += fmt::format("{}lfPitchAndFamily  = {}\n", indent(1), lfPitchAndFamily);
        str += fmt::format("{}lfFaceName        = {}\n", indent(1), lfFaceName);

        return str;
    }

    int32_t lfHeight;
    int32_t lfWidth;
    int32_t lfEscapement;
    int32_t lfOrientation;
    int32_t lfWeight;
    uint8_t lfItalic;
    uint8_t lfUnderline;
    uint8_t lfStrikeOut;
    uint8_t lfCharSet;
    uint8_t lfOutPrecision;
    uint8_t lfClipPrecision;
    uint8_t lfQuality;
    uint8_t lfPitchAndFamily;

    // Windows creates a fixed size buffer where the string is
    // copied into. However, when the string does not require the
    // full buffer size it contains still data from the previous
    // application. When the buffer is written into the file this
    // results in unintentionally writing unknown data. This might
    // cause some privacy issue since the data at the end of the
    // buffer should never end up in some file.
    // The code introducing this issues could look like this:
    // char buffer[60];
    // strcpy(buffer, srcStr);
    // write_data_to_file(buffer, sizeof(buffer));
    char    lfFaceName[32];
};


typedef tagLOGFONTA LOGFONTA, *PLOGFONTA, *NPLOGFONTA, *LPLOGFONTA;


// Check that structure size is as defined by the Windows API
static_assert(sizeof(LOGFONTA) == 60U);


#endif // LOGFONTA_HPP