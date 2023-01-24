#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Library.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"


void StructSymbolDisplayProp::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    auto_read_prefixes();

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    nameIdx = mDs.get().readUint32();

    // @todo move to left shift operator
    // @bug The required string is not this one but the value of the associated property!!!!
    //      This is just the name of the property!!
    spdlog::trace("strLst Item = {}", gLibrary->library->strLst.at(nameIdx));

    x = mDs.get().readInt16();
    y = mDs.get().readInt16();

    // Rotation and Font ID bit field
    struct RotFontIdBitField
    {
        RotFontIdBitField(uint16_t aInit)
        {
            textFontIdx = (aInit & 0x3fff) >>  0U;
            rotation    = (aInit & 0xc000) >> 14U;
        }

        uint16_t textFontIdx : 14; // 13 downto  0
        uint16_t rotation    :  2; // 15 downto 14
    };

    const RotFontIdBitField rotFontIdBitField{mDs.get().readUint16()};

    textFontIdx = rotFontIdBitField.textFontIdx;

    // @todo Sometimes values = textFonts.size() are observed that seem valid.
    //       Maybe they have a special meaning? I remember that somewhere in the
    //       code we had a similar issue. The solution was that the actual vector
    //       index is textFontIdx - 1 and index = 0 is a special case that represents
    //       an empty string i.e. does not need to be loaded from the vector.
    if(textFontIdx >= gLibrary->library->textFonts.size())
    {
        const std::string msg = fmt::format("{}: textFontIdx is out of range! Expected {} < {}!",
            __func__, textFontIdx, gLibrary->library->textFonts.size());

        spdlog::warn(msg);
        // throw std::out_of_range(msg);
    }

    rotation = ToRotation(rotFontIdBitField.rotation);

    propColor = ToColor(mDs.get().readUint8());

    // Somehow relates to the visiblity of text. See show "Value if Value exist" and the other options
    //        Do not display
    // cc 01  Value only
    // 00 02  Name and value
    // 00 03  Name only
    // 00 04  Both if value exist
    //        Value if value exist
    mDs.get().printUnknownData(2, std::string(__func__) + " - 0");

    mDs.get().assumeData({0x00}, std::string(__func__) + " - 1");

    sanitizeThisFutureSize(thisFuture);

    // readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::trace(to_string());
}