#ifndef STREAMFACTORY_HPP
#define STREAMFACTORY_HPP


#include <memory>
#include <string>
#include <vector>

#include "CommonBase.hpp"
#include "ParserContext.hpp"


class StreamFactory
{
public:
    static std::unique_ptr<CommonBase> build(ParserContext& aCtx, const std::vector<std::string>& aCfbfStreamLocation);
};


#endif // STREAMFACTORY_HPP