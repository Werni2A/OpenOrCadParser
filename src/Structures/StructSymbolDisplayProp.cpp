#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Parser.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"


StructSymbolDisplayProp Parser::readStructSymbolDisplayProp()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    readPreamble();

    const std::optional<FutureData> thisFuture = getFutureData();

    StructSymbolDisplayProp obj;

    obj.nameIdx = mDs.readUint32();

    // @todo move to left shift operator
    // @bug The required string is not this one but the value of the associated property!!!!
    //      This is just the name of the property!!
    spdlog::debug("strLst Item = {}", mLibrary.library.strLst.at(obj.nameIdx));

    obj.x = mDs.readInt16();
    obj.y = mDs.readInt16();

    // Rotation and Font ID bit field
    struct RotFontIdBitField
    {
        uint16_t textFontIdx : 14; // 13 downto  0
        uint16_t rotation    :  2; // 15 downto 14
    };

    const RotFontIdBitField rotFontIdBitField{mDs.readUint16()};

    obj.textFontIdx = rotFontIdBitField.textFontIdx;

    // @todo Sometimes values = textFonts.size() are observed that seem valid.
    //       Maybe they have a special meaning? I remember that somewhere in the
    //       code we had a similar issue. The solution was that the actual vector
    //       index is textFontIdx - 1 and index = 0 is a special case that represents
    //       an empty string i.e. does not need to be loaded from the vector.
    if(obj.textFontIdx >= mLibrary.library.textFonts.size())
    {
        const std::string msg = fmt::format("{}: textFontIdx is out of range! Expected {} < {}!",
            __func__, obj.textFontIdx, mLibrary.library.textFonts.size());

        spdlog::error(msg);
        // throw std::out_of_range(msg);
    }

    obj.rotation = ToRotation(rotFontIdBitField.rotation);

    obj.propColor = ToColor(mDs.readUint8());

    // Somehow relates to the visiblity of text. See show "Value if Value exist" and the other options
    //        Do not display
    // cc 01  Value only
    // 00 02  Name and value
    // 00 03  Name only
    // 00 04  Both if value exist
    //        Value if value exist
    mDs.printUnknownData(2, std::string(__func__) + " - 0");

    mDs.assumeData({0x00}, std::string(__func__) + " - 1");

    sanitizeThisFutureSize(thisFuture);

    readOptionalTrailingFuture();

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}