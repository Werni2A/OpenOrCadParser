#ifndef RECORDFACTORY_HPP
#define RECORDFACTORY_HPP


#include <memory>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "Record.hpp"
#include "StreamContext.hpp"


class RecordFactory
{
public:
    static std::unique_ptr<Record> build(StreamContext& aCtx, Structure aStructure);
    static std::unique_ptr<Record> build(StreamContext& aCtx, Primitive aPrimitive);
};


#endif // RECORDFACTORY_HPP