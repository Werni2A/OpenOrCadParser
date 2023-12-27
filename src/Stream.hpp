#ifndef STREAM_HPP
#define STREAM_HPP


#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include "ContainerContext.hpp"
#include "Enums/Structure.hpp"
#include "FutureData.hpp"
#include "General.hpp"
#include "Record.hpp"
#include "VisitorPattern.hpp"


class Stream : public Component
{
public:
    Stream(ContainerContext& aCtx, const fs::path& aInputStream) : mCtx{aCtx, aInputStream}
    { }

    virtual std::string to_string() const = 0;

    virtual void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) = 0;

    void openFile()
    {
        spdlog::info("Opening file: {}", mCtx.mInputStream.string());

        spdlog::info("File contains {} byte.", fs::file_size(mCtx.mInputStream));
    }

    void closeFile()
    {
        spdlog::info("Closing file: {}", mCtx.mInputStream.string());

        mCtx.mDs.close();
    }

// protected:
    StreamContext mCtx;
};


#endif // STREAM_HPP