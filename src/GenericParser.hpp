#ifndef GENERICPARSER_HPP
#define GENERICPARSER_HPP


#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Enums/Primitive.hpp"
#include "Enums/Structure.hpp"
#include "General.hpp"
#include "StreamContext.hpp"
// #include "Record.hpp"


class FutureDataLst;
class Record;


class GenericParser
{
public:

    GenericParser(StreamContext& aCtx) : mCtx{aCtx}
    { }

    void discard_until_preamble();

    Structure auto_read_prefixes(FutureDataLst& aFutureDataLst);

    Structure auto_read_prefixes(Structure aExpectedStruct, FutureDataLst& aFutureDataLst);

    Structure auto_read_prefixes(const std::vector<Structure>& aExpectedOneOfStruct, FutureDataLst& aFutureDataLst);

    Structure read_prefixes(size_t aNumber, FutureDataLst& aFutureDataLst);

    std::pair<Structure, uint32_t> read_single_prefix();

    std::pair<Structure, uint32_t> read_single_prefix_short();

    void readPreamble();
    Primitive readPrefixPrimitive();

    void checkInterpretedDataLen(const std::string &aFuncName, size_t aStartOffset, size_t aEndOffset, size_t aExpectedLen);

    FileFormatVersion predictVersion(std::function<void(FileFormatVersion)> aFunc);

    std::unique_ptr<Record> readPrimitive();
    std::unique_ptr<Record> readPrimitive(Primitive aPrimitive);

    std::unique_ptr<Record> readStructure();
    std::unique_ptr<Record> readStructure(Structure aStructure);

    // Return true if function call was successful, i.e. without throwing exceptions
    bool tryRead(std::function<void(void)> aFunction);

private:

    StreamContext& mCtx;
};


#endif // GENERICPARSER_HPP