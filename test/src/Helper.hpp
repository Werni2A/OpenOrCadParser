#ifndef HELPER_HPP
#define HELPER_HPP


#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include <spdlog/spdlog.h>

#include <Container.hpp>


namespace fs = std::filesystem;

using namespace std::chrono_literals;


[[maybe_unused]]
inline OOCP::ParserConfig get_parser_config()
{
    OOCP::ParserConfig cfg{};

    cfg.mThreadCount       = std::size_t{std::thread::hardware_concurrency()};
    cfg.mSkipUnknownPrim   = false;
    cfg.mSkipInvalidPrim   = false;
    cfg.mSkipUnknownStruct = false;
    cfg.mSkipInvalidStruct = false;
    cfg.mKeepTmpFiles      = false;

    return cfg;
}


[[maybe_unused]]
inline void configure_spdlog()
{
    spdlog::set_level(spdlog::level::off);

    spdlog::set_pattern("[%^%l%$] %v");
}


[[maybe_unused]]
inline void check_error_count(const fs::path& aFilePath, size_t aActualErrCnt, size_t aExpectedErrCnt)
{
    std::string msg;

    if(aActualErrCnt < aExpectedErrCnt)
    {
        msg = fmt::format("REDUCTION: {:>4} (actual errors) < {:>4} (expected errors) in {}\n",
            aActualErrCnt, aExpectedErrCnt, aFilePath.string());

        std::clog << msg;
    }

    if(aActualErrCnt > aExpectedErrCnt)
    {
        msg = fmt::format("INCREASE:  {:>4} (actual errors) > {:>4} (expected errors) in {}\n",
            aActualErrCnt, aExpectedErrCnt, aFilePath.string());

        std::clog << msg;
    }

    const fs::path errCntFilePath = fs::current_path() / "test_err_cnt.log";
    fs::path lockFilePath = errCntFilePath;
    lockFilePath += std::string{".lock"};

    // Ensure that we acquire the test_err_cnt.log.lock file before writing
    // to test_err_cnt.log to ensure syncronization across multiple processes
    // that want to write to this file.
    while(true)
    {
        std::FILE* lockFile = std::fopen(lockFilePath.c_str(), "ax");

        if(nullptr != lockFile)
        {
            std::ofstream errCntFile;

            errCntFile.open(errCntFilePath, std::ios_base::app);

            errCntFile << msg;

            errCntFile.close();

            std::fclose(lockFile);
            fs::remove(lockFilePath);

            break;
        }

        std::this_thread::sleep_for(25ms);
    }
}


#endif // HELPER_HPP