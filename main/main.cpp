#include <argparse/argparse.hpp>
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "bin_file_read.h"
#include "common.h"
#include "converters.h"
#include "eval.h"
#include "logger.h"

#define HXR(config, command, type, command_name)         \
    {                                                    \
        auto variable = command.get<type>(command_name); \
        converters::Hexer<type> hxr{variable};           \
        hxr.setConfig(config);                           \
        hxr.run();                                       \
    }

int main(int argc, char** argv) {
    argparse::ArgumentParser program("hxr");
    program.add_argument("--log-level")
        .help("Log level [V]erbose, D[ebug], I[nfo], W[arning]")
        .default_value("I");
    argparse::ArgumentParser query_command("query");
    argparse::ArgumentParser evaluate_command("evaluate");

    auto& group = query_command.add_mutually_exclusive_group();
    group.add_argument("-u").help("unsigned integer").scan<'u', uint64_t>();
    group.add_argument("-d").help("signed integer").scan<'d', int64_t>();
    group.add_argument("-t").help("text");
    group.add_argument("-x").help("hex value unsigned").scan<'x', uint64_t>();

    query_command.add_argument("-b", "--binary").help("Show binary?").flag();
    query_command.add_argument("-s")
        .default_value(uint16_t{4})
        .scan<'u', uint16_t>();
    program.add_subparser(query_command);

    auto& evaluate_group = evaluate_command.add_mutually_exclusive_group();
    evaluate_group.add_argument("--eval")
        .help("Evaluate an expression. Currently only hex is allowed")
        .flag();
    evaluate_command.add_argument("--file"); 

    program.add_subparser(query_command);
    program.add_subparser(evaluate_command);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    // get and set log-level
    common::Logger::getLogger().setSeverity(
        program.get<std::string>("--log-level"));


    if (program.is_subcommand_used("query")) {
        auto showBin = query_command.get<bool>("b");
        auto size = query_command.get<uint16_t>("s");
        bool isSigned = query_command.is_used("d");
        auto isText = query_command.is_used("t");
        auto isHex = query_command.is_used("x");

        auto metadata = converters::ArgumentMetadata{size}
                            .withShowBinary(showBin)
                            .withIsSigned(isSigned)
                            .withIsString(isText)
                            .withIsHex(isHex);

        if (isText) {
            HXR(metadata, query_command, std::string, "t");
            return 0;
        }
        if (isSigned) {
            HXR(metadata, query_command, int64_t, metadata.isHex ? "x" : "d");
        } else {
            HXR(metadata, query_command, uint64_t, metadata.isHex ? "x" : "u");
        }

        return 0;
    } else {
        // evaluate

        if (evaluate_command.is_used("--file")) {
            auto filename = evaluate_command.get<std::string>("file");
            LOG_D() << "file present: " << filename;
            BinFileRead bfr{filename};
            bfr.run<uint32_t>();

        } else {
            /// XXX: move this to subcommand... Likely the rest too
            std::cout << "<expression>: ";
            std::string input;
            std::getline(std::cin, input);
            std::cout << "input was: " << input << std::endl;
            Eval evaluation{input};
            return 0;
        }
    }

    // std::ifstream maps("/proc/self/maps");
    // std::string line;
    // while (std::getline(maps, line)) {
    //     std::cout << line << std::endl;
    //     std::cout << (void*)line.data() << std::endl;
    // }
    return 0;
}
