#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "Symbol.hpp"

#include "../Parser.hpp"


Symbol Parser::readSymbol()
{
    spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

    Symbol obj; // @todo make to symbol

    Structure structure;

    Structure introId = ToStructure(mDs.readUint8());

    switch(introId)
    {
        case Structure::GlobalSymbol:
        case Structure::PortSymbol:
        case Structure::OffPageSymbol:
        case Structure::PinShapeSymbol:
            mDs.printUnknownData(2, std::string(__func__) + " - 0");
            mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, std::string(__func__) + " - 1");
            // structure = read_prefixes(4);
            structure = auto_read_prefixes();
            break;

        case Structure::TitleBlockSymbol:
            readTitleBlockSymbol();
            return obj;
            break;

        default:
            throw std::runtime_error(fmt::format("{}: Unexpected Structure `{}`", __func__, to_string(introId)));
            break;
    }

    readConditionalPreamble(introId);
    pushStructure(parseStructure(structure), obj);

    readPreamble();

    if(introId == Structure::PinShapeSymbol)
    {
        readSymbolBBox(); // @todo push structure
    }
    else
    {
        mDs.printUnknownData(10, std::string(__func__) + " - 1.1");
    }

    // @todo how often does it repeat? This should be specified somewhere....
    for(size_t i = 0u; true; ++i)
    {
        if(mDs.isEoF())
        {
            spdlog::debug("i = {}", i);
            break;
        }

        // structure = read_prefixes(3);
        structure = auto_read_prefixes();
        readConditionalPreamble(structure);
        // readPreamble();
        pushStructure(parseStructure(structure), obj);

        mDs.printUnknownData(2, std::string(__func__) + " - 2");
    }

    if(!mDs.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
    spdlog::info(to_string(obj));

    return obj;
}