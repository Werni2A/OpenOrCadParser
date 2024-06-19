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

    mCtx.mLogger.debug(getOpeningMsg(getMethodName(this, __func__), ds.getCurrentOffset()));

    // Early out for empty caches
    if(parser.tryRead([&](){ ds.readBytes(10U); ds.sanitizeEoF(); }))
    {
        // 10 zero bytes
        ds.assumeData({0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, getMethodName(this, __func__) + ": 0");
    }
    else
    {
        ds.printUnknownData(4, getMethodName(this, __func__) + ": 1");

        std::size_t i{0U};
        for(i = 0U; !ds.isEoF(); ++i)
        {
            mCtx.mLogger.trace("iteration i = {}", i);

            const bool hasStrAfter8Byte = parser.tryRead([&](){ ds.readBytes(8U); ds.readStringLenZeroTerm(); });
            const bool hasStrAfter0Byte = parser.tryRead([&](){ ds.readBytes(0U); ds.readStringLenZeroTerm(); });

            mCtx.mLogger.trace("hasStrAfter8Byte = {}", hasStrAfter8Byte);
            mCtx.mLogger.trace("hasStrAfter0Byte = {}", hasStrAfter0Byte);

            if(!hasStrAfter0Byte)
            {
                if(hasStrAfter8Byte)
                {
                    ds.printUnknownData(2U, getMethodName(this, __func__) + ": 11");

                    // Some 3 letter descriptions like: "LED", "VDC", "POT", "USB", "BUF", ...
                    const std::string someRefDes = ds.readStringLenZeroTerm();
                    mCtx.mLogger.trace("someRefDes = {}", someRefDes);
                }

                ds.printUnknownData(2U, getMethodName(this, __func__) + ": 10");
            }

            const std::string name = ds.readStringLenZeroTerm();

            mCtx.mLogger.trace("name = {}", name);

            const auto ids = ds.peek(8U);
            const std::vector<uint8_t> id0{ids.cbegin(), ids.cbegin() + 4U};
            const std::vector<uint8_t> id1{ids.cbegin() + 4U, ids.cend()};

            if(id0 != id1)
            {
                uint16_t someVal;
                do
                {
                    // Indicates what follows
                    // 0 = Package Name
                    // 1 = Probably Source Library
                    // 2 = Probably Source Library
                    // 3 = Probably Source Library
                    // Other values have not been observed
                    someVal = ds.readUint16();

                    mCtx.mLogger.trace("someVal = {}", someVal);

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

                    mCtx.mLogger.trace("someStr[{}] = {}", i, someStr);
                } while (someVal == 0x0U);

                if(ds.isEoF())
                {
                    break;
                }
            }

            const uint32_t some_id0 = ds.readUint32();
            mCtx.mLogger.trace("some_id0 = {}", some_id0);
            const uint32_t some_id1 = ds.readUint32();
            mCtx.mLogger.trace("some_id1 = {}", some_id1);

            if(some_id0 != some_id1)
            {
                mCtx.mLogger.error("some_id0 ({}) != some_id1 ({})", some_id0, some_id1);
            }

            const Structure my_struct = ToStructure(ds.readUint16());
            mCtx.mLogger.trace(::to_string(my_struct));

            parser.readStructure();
        }

        mCtx.mLogger.trace("unknownLen5176 = {}", i);
    }

    ds.sanitizeEoF();

    mCtx.mLogger.debug(getClosingMsg(getMethodName(this, __func__), ds.getCurrentOffset()));
    mCtx.mLogger.info(to_string());
}