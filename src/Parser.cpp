#include <algorithm>
#include <any>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "ContainerExtractor.hpp"
#include "DataStream.hpp"
#include "General.hpp"
#include "Parser.hpp"


namespace fs = std::filesystem;


Parser::Parser(const fs::path& aFile)
{
    mFileType      = getFileTypeByExtension(aFile);
    mInputFile     = aFile;
    mInputFileSize = fs::file_size(aFile);

    const fs::path extractTo = fs::temp_directory_path() / "OpenOrCadParser";
    mExtractedPath = extractContainer(aFile, extractTo);

    // @todo Figure out the file format version and set it here
    mFileFormatVersion = FileFormatVersion::Unknown;
}


fs::path Parser::extractContainer(const fs::path& aFile, const fs::path& aOutDir) const
{
    ContainerExtractor extractor{aFile};

    return extractor.extract(aOutDir);
}


fs::path Parser::extractContainer(const fs::path& aOutDir) const
{
    return extractContainer(mInputFile, aOutDir);
}


void Parser::printContainerTree() const
{
    ContainerExtractor extractor{mInputFile};
    extractor.printContainerTree();
}


FileType Parser::getFileTypeByExtension(const fs::path& aFile) const
{
    std::string extension = aFile.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(),
        [](unsigned char c){ return std::toupper(c); });

    FileType fileType;

    if(extension == ".OLB" ||
       extension == ".OBK")
    {
        fileType = FileType::Library;
    }
    else if(extension == ".DSN" ||
        extension == ".DBK")
    {
        fileType = FileType::Schematic;
    }
    else
    {
        throw std::runtime_error("Unknown file extension: " + extension);
    }
    return fileType;
}


void Parser::openFile(const fs::path& aFile)
{
    std::cout << "Opening file: " << aFile << std::endl;

    mDs = DataStream{aFile};
    if(!mDs)
    {
        throw std::runtime_error("Could not open file: " + aFile.string());
    }

    mCurrOpenFile     = aFile;
    mCurrOpenFileSize = fs::file_size(aFile);

    std::cout << "File contains " << mCurrOpenFileSize << " byte." << std::endl;
}


void Parser::closeFile()
{
    std::cout << "Closing file: " << mCurrOpenFile << std::endl;

    mDs.close();

    mCurrOpenFile.clear();
    mCurrOpenFileSize = 0u;
}