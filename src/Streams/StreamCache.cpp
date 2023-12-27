#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "Enums/Structure.hpp"
#include "General.hpp"
#include "GenericParser.hpp"
#include "Streams/StreamCache.hpp"


void StreamCache::read(FileFormatVersion /* aVersion */)
{
    auto& ds = mCtx.mDs;
    GenericParser parser{mCtx};

    spdlog::debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    // Early out for empty caches
    if(parser.tryRead([&](){ ds.readBytes(10U); ds.sanitizeEoF(); }))
    {
        ds.printUnknownData(10U, getMethodName(this, __func__) + ": 0");
    }
    else
    {
        ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");

        for(std::size_t i{0U}; !ds.isEoF(); ++i)
        {
            spdlog::trace("iteration i = {}", i);

            const bool hasStrAfter8Byte = parser.tryRead([&](){ ds.readBytes(8U); ds.readStringLenZeroTerm(); });
            const bool hasStrAfter2Byte = parser.tryRead([&](){ ds.readBytes(2U); ds.readStringLenZeroTerm(); });
            const bool hasStrAfter0Byte = parser.tryRead([&](){ ds.readBytes(0U); ds.readStringLenZeroTerm(); });

            spdlog::trace("hasStrAfter8Byte = {}", hasStrAfter8Byte);
            spdlog::trace("hasStrAfter2Byte = {}", hasStrAfter2Byte);
            spdlog::trace("hasStrAfter0Byte = {}", hasStrAfter0Byte);

            if(!hasStrAfter0Byte)
            {
                if(hasStrAfter8Byte)
                {
                    ds.printUnknownData(8U, getMethodName(this, __func__) + ": 11");
                }

                if(hasStrAfter2Byte)
                {
                    ds.printUnknownData(2U, getMethodName(this, __func__) + ": 10");
                }
            }

            const std::string name = ds.readStringLenZeroTerm();

            spdlog::trace("name = {}", name);

            const auto ids = ds.peek(8U);
            const std::vector<uint8_t> id0{ids.cbegin(), ids.cbegin() + 4U};
            const std::vector<uint8_t> id1{ids.cbegin() + 4U, ids.cend()};

            if(id0 != id1)
            {
                uint16_t someVal;
                do
                {
                    someVal = ds.readUint16();

                    spdlog::trace("someVal = {}", someVal);

                    if(ds.isEoF())
                    {
                        break;
                    }

                    const bool hasMysterious2Byte = !parser.tryRead([&](){ ds.readStringLenZeroTerm(); });

                    if(hasMysterious2Byte)
                    {
                        ds.printUnknownData(2U, "Encountered weird values");
                    }

                    const std::string someStr = ds.readStringLenZeroTerm();

                    spdlog::trace("someStr[{}] = {}", i, someStr);
                } while (someVal == 0x0U);

                if(ds.isEoF())
                {
                    break;
                }
            }

            const uint32_t some_id0 = ds.readUint32();
            spdlog::trace("some_id0 = {}", some_id0);
            const uint32_t some_id1 = ds.readUint32();
            spdlog::trace("some_id1 = {}", some_id1);

            if(some_id0 != some_id1)
            {
                spdlog::error("some_id0 ({}) != some_id1 ({})", some_id0, some_id1);
            }

            const Structure my_struct = ToStructure(ds.readUint16());
            spdlog::trace(::to_string(my_struct));

            parser.readStructure();
        }
    }

    if(!ds.isEoF())
    {
        throw std::runtime_error("Expected EoF but did not reach it!");
    }

    spdlog::debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    spdlog::info(to_string());
}