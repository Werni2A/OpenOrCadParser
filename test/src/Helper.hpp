#ifndef HELPER_H
#define HELPER_H


#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>


namespace fs = std::filesystem;


[[maybe_unused]]
inline void check_error_count(const fs::path& aFilePath, size_t aActualErrCnt, size_t aExpectedErrCnt)
{
    if(aActualErrCnt < aExpectedErrCnt)
    {
        std::clog << std::to_string(aActualErrCnt) << " (actual errors) < "
                  << std::to_string(aExpectedErrCnt) << " (expected errors) in "
                  << aFilePath.string() << std::endl;
    }

    const fs::path logPath = fs::current_path() / "test_err_cnt.log";

    std::ofstream logFile;

    logFile.open(logPath, std::ios_base::app);
    logFile << "Expected " << std::setw(4) << aExpectedErrCnt
            << " errors and got " << std::setw(4) << aActualErrCnt
            << " errors in `" << aFilePath.string() << "`\n";
}


#endif // HELPER_H