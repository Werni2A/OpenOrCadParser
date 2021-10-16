#ifndef PARSER_H
#define PARSER_H


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "DataStream.hpp"
#include "General.hpp"


namespace fs = std::filesystem;


class Parser
{
public:

    Parser(const fs::path& aFile);

    /**
     * @brief Extract container.
     *
     * @param aFile Path to container file.
     * @param aOutDir Output directory.
     * @return fs::path Path to the extraction directory.
     */
    fs::path extractContainer(const fs::path& aFile, const fs::path& aOutDir) const;

    /**
     * @brief Extract currently used container.
     *
     * @param aOutDir Output directory.
     * @return fs::path Path to the extraction directory.
     */
    fs::path extractContainer(const fs::path& aOutDir) const;

    /**
     * @brief Print container tree structure to console.
     */
    void printContainerTree() const;

    /**
     * @brief Get the file type from file extension.
     *
     * @param aFile File name including its extension.
     * @return FileType Type of the file.
     */
    FileType getFileTypeByExtension(const fs::path& aFile) const;

private:

    void openFile(const fs::path& aFile);
    void closeFile();

    FileType mFileType;
    FileFormatVersion mFileFormatVersion;

    fs::path mInputFile;
    size_t   mInputFileSize;

    fs::path mCurrOpenFile;
    size_t   mCurrOpenFileSize;

    fs::path mExtractedPath;

    DataStream mDs;
};


#endif // PARSER_H