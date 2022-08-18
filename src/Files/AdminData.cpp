#include <cstdint>
#include <string>

#include <nameof.hpp>
#include <spdlog/spdlog.h>

#include "../General.hpp"

#include "AdminData.hpp"

#include "../Parser.hpp"


void Parser::readAdminData(const fs::path& aFilePath)
{
    // @todo probably move all this mFileCtr and try-catch block
    //       stuff into a separate function
    ++mFileCtr;
    try
    {
        openFile(aFilePath);

        spdlog::debug(getOpeningMsg(__func__, mDs.getCurrentOffset()));

        AdminData obj;

        try{
        mDs.assumeData({0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, fmt::format("{}: Found actually useful data!", __func__));
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }

        if(!mDs.isEoF())
        {
            // @todo use function
            throw std::runtime_error("Exptected EoF in AdminData");
        }

        spdlog::debug(getClosingMsg(__func__, mDs.getCurrentOffset()));
        spdlog::info(to_string(obj));

        // return obj;

        closeFile();
    }
    catch(...)
    {
        exceptionHandling();
    }

    spdlog::info("\n----------------------------------------------------------------------------------\n");
}