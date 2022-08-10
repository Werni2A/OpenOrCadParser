#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "../General.hpp"
#include "../Parser.hpp"
#include "TextFont.hpp"


TextFont Parser::readTextFont()
{
    std::clog << getOpeningMsg(__func__, mDs.getCurrentOffset()) << std::endl;

    const size_t startOffset = mDs.getCurrentOffset();

    TextFont obj;

    obj.height = mDs.readInt32();
    obj.width  = mDs.readInt32();

    obj.escapement = mDs.readUint16(); // @todo not sure

    mDs.printUnknownData(6, std::string(__func__) + " - 0");

    obj.weight = mDs.readUint16();

    mDs.printUnknownData(2, std::string(__func__) + " - 1");

    obj.italic = mDs.readUint16(); // @todo not sure

    mDs.printUnknownData(6, std::string(__func__) + " - 2");

    obj.fontName = mDs.readStringZeroTerm();
    // Looks like OrCAD creates a fixed size buffer where the string
    // is copied into. However, when the string does not requrie the
    // full buffer size it contains still data from the previous
    // application. When the buffer is written into the file this
    // probably causes some privacy issues as the data at the end
    // of the buffer should never end up in the symbols file.
    // Some OrCAD code like this would cause the issue:
    // char buffer[60];
    // strcpy(buffer, srcStr);
    // write_data_to_file(buffer, sizeof(buffer));
    mDs.padRest(startOffset, 60, false);

    static size_t ctr = 0u;
    std::cout << "Index = " << std::to_string(ctr) << std::endl;
    ctr++;

    std::clog << getClosingMsg(__func__, mDs.getCurrentOffset()) << std::endl;
    std::clog << obj << std::endl;

    return obj;
}