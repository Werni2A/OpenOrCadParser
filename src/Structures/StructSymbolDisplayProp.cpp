#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "Database.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "GetStreamHelper.hpp"
#include "Structures/StructSymbolDisplayProp.hpp"

void OOCP::StructSymbolDisplayProp::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    const auto lib = getLibraryStreamFromDb(mCtx.mDb);

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    FutureDataLst localFutureLst{mCtx};

    parser.auto_read_prefixes(Structure::SymbolDisplayProp, localFutureLst);

    parser.readPreamble();

    localFutureLst.checkpoint();

    nameIdx = ds.readUint32();

    // @todo move to left shift operator
    // @bug The required string is not this one but the value of the associated property!!!!
    //      This is just the name of the property!!
    if(lib)
    {
        if(nameIdx < lib->strLst.size())
        {
            mCtx.mLogger.trace("strLst Item @ {} = {}", nameIdx, lib->strLst.at(nameIdx));
        }
        else
        {
            mCtx.mLogger.warn("Index is out-of-range: {} vs {}", nameIdx, lib->strLst.size());
        }
    }

    x = ds.readInt16();
    y = ds.readInt16();

    // Rotation and Font ID bit field
    struct RotFontIdBitField
    {
        RotFontIdBitField(uint16_t aInit)
        {
            textFontIdx = (aInit & 0x3fff) >> 0U;
            rotation    = (aInit & 0xc000) >> 14U;
        }

        uint16_t textFontIdx : 14; // 13 downto  0
        uint16_t rotation : 2;     // 15 downto 14
    };

    const RotFontIdBitField rotFontIdBitField{ds.readUint16()};

    textFontIdx = rotFontIdBitField.textFontIdx;

    // @todo Sometimes values = textFonts.size() are observed that seem valid.
    //       Maybe they have a special meaning? I remember that somewhere in the
    //       code we had a similar issue. The solution was that the actual vector
    //       index is textFontIdx - 1 and index = 0 is a special case that represents
    //       an empty string i.e. does not need to be loaded from the vector.
    if(lib)
    {
        if(textFontIdx >= lib->textFonts.size())
        {
            const std::string msg = fmt::format(
                "{}: textFontIdx is out of range! Expected {} < {}!", __func__, textFontIdx, lib->textFonts.size());

            mCtx.mLogger.warn(msg);
            // throw std::out_of_range(msg);
        }
    }

    rotation = ToRotation(rotFontIdBitField.rotation);

    propColor = ToColor(ds.readUint8());

    // Somehow relates to the visiblity of text. See show "Value if Value exist" and the other options
    //        Do not display
    // cc 01  Value only
    // 00 02  Name and value
    // 00 03  Name only
    // 00 04  Both if value exist
    //        Value if value exist
    ds.printUnknownData(2, getMethodName(this, __func__) + ": 0");

    ds.assumeData({0x00}, getMethodName(this, __func__) + ": 1");

    localFutureLst.checkpoint();

    localFutureLst.sanitizeCheckpoints();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.trace(to_string());
}