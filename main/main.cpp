#include <pthread.h>

#include <argparse/argparse.hpp>
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "common.h"
#include "converters.h"

int main(int argc, char** argv) {
    argparse::ArgumentParser program("hxr");

    auto& group = program.add_mutually_exclusive_group();
    group.add_argument("-u").help("unsigned integer").scan<'u', uint64_t>();
    group.add_argument("-d").help("signed integer").scan<'d', int64_t>();
    group.add_argument("-t").help("text");
    group.add_argument("-x").help("hex value unsigned").scan<'x', uint64_t>();
    group.add_argument("--eval")
        .help("Evaluate an expression. Currently only hex is allowed")
        .flag();

    program.add_argument("-b", "--binary").help("Show binary?").flag();
    program.add_argument("-l", "--long").help("Is 64-bit? (long)").flag();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    auto evalProgram = program.get<bool>("eval");

    if (evalProgram) {
        /// XXX: move this to subcommand... Likely the rest too
        std::cout << "<expression>: ";
        std::string input;
        std::getline(std::cin, input);
        std::cout << "input was: " << input << std::endl;
        Eval evaluation{input};
        return 0;
    }

    auto showBin = program.get<bool>("b");
    auto is64Bit = program.get<bool>("l");
    bool isSigned = program.is_used("d");
    auto isText = program.is_used("t");
    auto isHex = program.is_used("x");

    if (isText) {
        auto text = program.get<std::string>("t");
        converters::Hexer<std::string> hxr{text};
        hxr.setShowBinary(showBin);
        hxr.setIsString(true);
        hxr.run();
        return 0;
    }

    if (isHex) {
        if (is64Bit) {
            auto number = program.get<uint64_t>("x");
            converters::Hexer<uint64_t> hxr{number};
            hxr.setIsHex(isHex);
            hxr.setShowBinary(showBin);
            hxr.run();
        } else {
            uint32_t number = static_cast<uint32_t>(program.get<uint64_t>("x"));
            converters::Hexer<uint32_t> hxr{number};
            hxr.setIsHex(isHex);
            hxr.setShowBinary(showBin);
            hxr.run();
        }
        return 0;
    }

    if (isSigned) {
        if (is64Bit) {
            int64_t number = static_cast<int64_t>(program.get<int64_t>("d"));
            converters::Hexer<int64_t> hxr{number};
            hxr.setShowBinary(showBin);
            hxr.run();
        } else {
            int32_t number = static_cast<int32_t>(program.get<int64_t>("d"));
            converters::Hexer<int32_t> hxr{number};
            hxr.setShowBinary(showBin);
            hxr.run();
        }
    } else {
        if (is64Bit) {
            auto number = program.get<uint64_t>("u");
            converters::Hexer<uint64_t> hxr{number};
            hxr.setShowBinary(showBin);
            hxr.run();
        } else {
            uint32_t number = static_cast<uint32_t>(program.get<uint64_t>("u"));
            converters::Hexer<uint32_t> hxr{number};
            hxr.setShowBinary(showBin);
            hxr.run();
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
