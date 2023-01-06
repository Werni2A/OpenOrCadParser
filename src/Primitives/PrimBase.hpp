#ifndef PRIMBASE_HPP
#define PRIMBASE_HPP


#include "CommonBase.hpp"
#include "DataStream.hpp"


class PrimBase : public CommonBase
{
public:
    PrimBase(DataStream& aDs) : CommonBase{aDs}
    { }
};


#endif // PRIMBASE_HPP