#ifndef RECORD_HPP
#define RECORD_HPP


#include <string>

#include "General.hpp"
#include "StreamContext.hpp"


class Record
{
public:
    Record(StreamContext& aCtx) : mCtx{aCtx}
    { }

    virtual std::string to_string() const = 0;

    virtual void read(FileFormatVersion aVersion = FileFormatVersion::Unknown) = 0;

protected:
    StreamContext& mCtx;
};


#endif // RECORD_HPP