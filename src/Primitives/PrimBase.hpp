#ifndef PRIMBASE_HPP
#define PRIMBASE_HPP


#include "CommonBase.hpp"
#include "DataStream.hpp"


class PrimBase : public CommonBase
{
public:
    PrimBase(ParserContext& aCtx) : CommonBase{aCtx}
    { }
};


#endif // PRIMBASE_HPP