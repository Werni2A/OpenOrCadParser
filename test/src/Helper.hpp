#ifndef HELPER_H
#define HELPER_H


#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <spdlog/spdlog.h>


namespace fs = std::filesystem;


[[maybe_unused]]
inline void configure_spdlog()
{
    spdlog::set_level(spdlog::level::off);

    spdlog::set_pattern("[%^%l%$] %v");
}


[[maybe_unused]]
inline void check_error_count(const fs::path& aFilePath, size_t aActualErrCnt, size_t aExpectedErrCnt)
{
    if(aActualErrCnt < aExpectedErrCnt)
    {
        const std::string msg = fmt::format("REDUCTION: {:>4} (actual errors) < {:>4} (expected errors) in {}",
            aActualErrCnt, aExpectedErrCnt, aFilePath.string());

        std::clog << msg << std::endl;
    }

    if(aActualErrCnt > aExpectedErrCnt)
    {
        const std::string msg = fmt::format("INCREASE:  {:>4} (actual errors) > {:>4} (expected errors) in {}",
            aActualErrCnt, aExpectedErrCnt, aFilePath.string());

        std::clog << msg << std::endl;
    }

    const fs::path logPath = fs::current_path() / "test_err_cnt.log";

    std::ofstream logFile;

    logFile.open(logPath, std::ios_base::app);

    const std::string msg = fmt::format(
        "Expected {:>4} errors and got {:>4} errors in `{}`\n",
        aExpectedErrCnt, aActualErrCnt, aFilePath.string());

    logFile << msg;
}


#endif // HELPER_H