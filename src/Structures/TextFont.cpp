#include <cstdint>
#include <ostream>
#include <string>

#include <nameof.hpp>

#include "General.hpp"
#include "Structures/TextFont.hpp"


// @todo this is not a Structure in this kind of context
//       Move it to somewhere else
void TextFont::read(FileFormatVersion /* aVersion */)
{
    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));

    const size_t startOffset = mDs.get().getCurrentOffset();

    height = mDs.get().readInt32();
    width  = mDs.get().readInt32();

    escapement = mDs.get().readUint16(); // @todo not sure

    mDs.get().printUnknownData(6, std::string(__func__) + " - 0");

    weight = mDs.get().readUint16();

    mDs.get().printUnknownData(2, std::string(__func__) + " - 1");

    italic = mDs.get().readUint16(); // @todo not sure

    mDs.get().printUnknownData(6, std::string(__func__) + " - 2");

    fontName = mDs.get().readStringZeroTerm();
    // Looks like OrCAD creates a fixed size buffer where the string
    // is copied into. However, when the string does not require the
    // full buffer size it contains still data from the previous
    // application. When the buffer is written into the file this
    // probably causes some privacy issues as the data at the end
    // of the buffer should never end up in the symbols file.
    // Some OrCAD code like this would cause the issue:
    // char buffer[60];
    // strcpy(buffer, srcStr);
    // write_data_to_file(buffer, sizeof(buffer));
    mDs.get().padRest(startOffset, 60, false);

    static size_t ctr = 0u;
    spdlog::debug("Index = {}", ctr);
    ctr++;

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), mDs.get().getCurrentOffset()));
    spdlog::info(to_string());
}