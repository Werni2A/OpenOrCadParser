#ifndef FUTUREDATA_HPP
#define FUTUREDATA_HPP

#include <algorithm>
#include <stdexcept>
#include <optional>
#include <vector>

#include <fmt/core.h>


class FutureData
{
public:

    FutureData(std::size_t aAbsStartOffset, std::size_t aAbsStopOffset)
        : mAbsStartOffset{aAbsStartOffset}, mAbsStopOffset{aAbsStopOffset}
    {
        sanitizeInputs();
    }

    FutureData(std::size_t aAbsStartPreambleOffset, std::size_t aRelStartOffset, std::size_t aAbsStopPreambleOffset, std::size_t aRelStopOffset)
    {
        const size_t PREAMBLE_STRIDE = 9U; //!< Stride from one preamble to the next one in byte

        if(aAbsStartPreambleOffset <= aAbsStopPreambleOffset)
        {
            spdlog::error("{}: StopPreamble always appears before the StartPreamble!", __func__);
        }

        mAbsStartOffset = aAbsStartPreambleOffset + PREAMBLE_STRIDE + aRelStartOffset;
        mAbsStopOffset  = aAbsStopPreambleOffset  + PREAMBLE_STRIDE + aRelStopOffset;

        spdlog::debug("{}: aAbsStartPreambleOffset = {:08x}; Start Offset = {}",
            __func__, aAbsStartPreambleOffset, aRelStartOffset);
        spdlog::debug("{}: aAbsStopPreambleOffset  = {:08x}; Stop Offset  = {}",
            __func__, aAbsStopPreambleOffset, aRelStopOffset);
        spdlog::debug("{}: Adding 0x{:08x} -> 0x{:08x}", __func__, mAbsStartOffset, mAbsStopOffset);

        sanitizeInputs();
    }

    std::size_t getStartOffset() const
    {
        return mAbsStartOffset;
    }

    std::size_t getStopOffset() const
    {
        return mAbsStopOffset;
    }

    std::size_t getByteLen() const
    {
        return mAbsStopOffset - mAbsStartOffset;
    }

    std::string string() const
    {
        return fmt::format("0x{:08x} -> 0x{:08x}: {} Byte", getStartOffset(), getStopOffset(), getByteLen());
    }

private:

    void sanitizeInputs() const
    {
        if(mAbsStopOffset < mAbsStartOffset)
        {
            throw std::runtime_error("StopOffset < StartOffset is not valid!");
        }
    }

    std::size_t mAbsStartOffset; //!< Absolute offset from the beginning of the file where the data starts
    std::size_t mAbsStopOffset; //!< Absolute offset from the beginnig of the file where the data ends
};



class FutureDataLst : public std::vector<FutureData>
{
public:
    std::optional<FutureData> getByStartOffset(std::size_t aAbsStartOffset) const
    {
        spdlog::info("Searching for StartOffset 0x{:08x}", aAbsStartOffset);

        const auto cmp = [&aAbsStartOffset] (FutureData aFutureData) -> bool
            { return aFutureData.getStartOffset() == aAbsStartOffset; };
        const auto res = std::find_if(this->begin(), this->end(), cmp);

        if(res != this->end())
        {
            return std::make_optional<FutureData>(*res);
        }

        return std::nullopt;
    }

    bool removeByStartOffset(std::size_t aAbsStartOffset)
    {
        const auto cmp = [&aAbsStartOffset] (FutureData aFutureData) -> bool
            { return aFutureData.getStartOffset() == aAbsStartOffset; };
        const auto res = std::find_if(this->begin(), this->end(), cmp);

        if(res != this->end())
        {
            this->erase(res);
            return true;
        }

        return false;
    }

};

#endif // FUTUREDATA_HPP