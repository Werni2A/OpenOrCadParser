#include <filesystem>
#include <string>

#include <boost/program_options.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Parser.hpp"


namespace fs = std::filesystem;
namespace po = boost::program_options;


void parseArgs(int argc, char* argv[], fs::path& input, bool& printTree, bool& extract, fs::path& output, int& verbosity)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h",                                                "produce help message")
        ("print_tree,t", po::bool_switch()->default_value(false), "print container tree")
        ("extract,e",    po::bool_switch()->default_value(false), "extract binary files from CFBF container")
        ("input,i",      po::value<std::string>(),                "input file to parse")
        ("output,o",     po::value<std::string>(),                "output path (required iff extract is set)")
        ("verbosity,v",  po::value<int>()->default_value(3),      "verbosity level (0 = off, 6 = highest)")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(vm.count("help") > 0U)
    {
        std::cout << desc << std::endl;
        std::exit(1);
    }

    printTree = vm.count("print_tree") ? vm["print_tree"].as<bool>() : false;
    extract   = vm.count("extract") ? vm["extract"].as<bool>() : false;
    verbosity = vm.count("verbosity") ? vm["verbosity"].as<int>() : 3;

    if(vm.count("input") > 0U)
    {
        input = fs::path{vm["input"].as<std::string>()};
        if(!fs::exists(input))
        {
            std::cout << "The following input file was not found: " << input.string() << std::endl;
            std::cout << desc << std::endl;
            std::exit(1);
        }

        if(!fs::is_regular_file(input))
        {
            std::cout << "The following input is not a file: " << input.string() << std::endl;
            std::cout << desc << std::endl;
            std::exit(1);
        }
    }
    else
    {
        std::cout << "input_file was not specified but is required." << std::endl;
        std::cout << desc << std::endl;
        std::exit(1);
    }

    if(vm.count("output") > 0U)
    {
        output = fs::path{vm["output"].as<std::string>()};
        if(!fs::exists(output))
        {
            std::cout << "The following output directory was not found: " << output.string() << std::endl;
            std::cout << desc << std::endl;
            std::exit(1);
        }

        if(!fs::is_directory(output))
        {
            std::cout << "The following output path is not a directory: " << output.string() << std::endl;
            std::cout << desc << std::endl;
            std::exit(1);
        }
    }
    else if(extract)
    {
        std::cout << "output was not specified but is required." << std::endl;
        std::cout << desc << std::endl;
        std::exit(1);
    }
}


int main(int argc, char* argv[])
{
    fs::path inputFile;
    bool     printTree;
    bool     extract;
    fs::path outputPath;
    int      verbosity;

    parseArgs(argc, argv, inputFile, printTree, extract, outputPath, verbosity);

   // Creating console logger
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    // Creating file logger
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("OpenOrCadParser.log");

    // Creating multi-logger
    spdlog::logger logger{"console and file", {console_sink, file_sink}};

    spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));

    switch(verbosity)
    {
        case 0: spdlog::set_level(spdlog::level::off);      break;
        case 1: spdlog::set_level(spdlog::level::critical); break;
        case 2: spdlog::set_level(spdlog::level::err);      break;
        case 3: spdlog::set_level(spdlog::level::warn);     break;
        case 4: spdlog::set_level(spdlog::level::info);     break;
        case 5: spdlog::set_level(spdlog::level::debug);    break;
        case 6: spdlog::set_level(spdlog::level::trace);    break;
        default:
            throw std::runtime_error(
                fmt::format("Invalid verbosity argument {}", verbosity));
            break;
    }

    spdlog::set_pattern("[%^%l%$] %v");

    Parser parser{inputFile};

    if(printTree)
    {
        parser.printContainerTree();
    }

    if(extract)
    {
        parser.extractContainer(outputPath);
    }

    if(!printTree && !extract)
    {
        parser.parseLibrary();
    }

    return 0;
}