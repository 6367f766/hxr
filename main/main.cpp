#include <argparse/argparse.hpp>
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "common.h"
#include "converters.h"
#include "eval.h"
#include "type_list.h"

int main(int argc, char** argv) {
    argparse::ArgumentParser program("hxr");
    argparse::ArgumentParser query_command("query");
    argparse::ArgumentParser evaluate_command("evaluate");

    auto& group = query_command.add_mutually_exclusive_group();
    group.add_argument("-u").help("unsigned integer").scan<'u', uint64_t>();
    group.add_argument("-d").help("signed integer").scan<'d', int64_t>();
    group.add_argument("-t").help("text");
    group.add_argument("-x").help("hex value unsigned").scan<'x', uint64_t>();

    query_command.add_argument("-b", "--binary").help("Show binary?").flag();
    query_command.add_argument("-l", "--long").help("Is 64-bit? (long)").flag();
    program.add_subparser(query_command);

    auto& evaluate_group = evaluate_command.add_mutually_exclusive_group();
    evaluate_group.add_argument("--eval")
        .help("Evaluate an expression. Currently only hex is allowed")
        .flag();

    program.add_subparser(query_command);
    program.add_subparser(evaluate_command);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    if (program.is_subcommand_used("query")) {
        auto showBin = query_command.get<bool>("b");
        auto is64Bit = query_command.get<bool>("l");
        bool isSigned = query_command.is_used("d");
        auto isText = query_command.is_used("t");
        auto isHex = query_command.is_used("x");

        if (isText) {
            constexpr char* argument = "t";
            using T = EnumToType_t<MapGet(argument)>;
            auto text = query_command.get<T>(argument);
            converters::Hexer<T> hxr{text};
            hxr.setShowBinary(showBin);
            hxr.setIsString(true);
            hxr.run();
            return 0;
        }

        if (isHex) {
            if (is64Bit) {
                auto number = query_command.get<uint64_t>("x");
                converters::Hexer<decltype(number)> hxr{number};
                hxr.setIsHex(isHex);
                hxr.setShowBinary(showBin);
                hxr.run();
            } else {
                uint32_t number =
                    static_cast<uint32_t>(query_command.get<uint64_t>("x"));
                converters::Hexer<uint32_t> hxr{number};
                hxr.setIsHex(isHex);
                hxr.setShowBinary(showBin);
                hxr.run();
            }
            return 0;
        }

        if (isSigned) {
            // signed
            auto number = query_command.get<int64_t>("d");
            converters::Hexer<decltype(number)> hxr{number};
            hxr.setIs64Bit(is64Bit);
            hxr.setShowBinary(showBin);
            hxr.run();

            if (is64Bit) {
                int64_t number =
                    static_cast<int64_t>(query_command.get<int64_t>("d"));
                converters::Hexer<int64_t> hxr{number};
                hxr.setShowBinary(showBin);
                hxr.run();
            } else {
                int32_t number =
                    static_cast<int32_t>(query_command.get<int64_t>("d"));
                converters::Hexer<int32_t> hxr{number};
                hxr.setShowBinary(showBin);
                hxr.run();
            }
        } else {
            if (is64Bit) {
                auto number = query_command.get<uint64_t>("u");
                converters::Hexer<uint64_t> hxr{number};
                hxr.setShowBinary(showBin);
                hxr.run();
            } else {
                uint32_t number =
                    static_cast<uint32_t>(query_command.get<uint64_t>("u"));
                converters::Hexer<uint32_t> hxr{number};
                hxr.setShowBinary(showBin);
                hxr.run();
            }
        }
        return 0;
    } else {
        // evaluate
        /// XXX: move this to subcommand... Likely the rest too
        std::cout << "<expression>: ";
        std::string input;
        std::getline(std::cin, input);
        std::cout << "input was: " << input << std::endl;
        Eval evaluation{input};
        return 0;
    }

    // std::ifstream maps("/proc/self/maps");
    // std::string line;
    // while (std::getline(maps, line)) {
    //     std::cout << line << std::endl;
    //     std::cout << (void*)line.data() << std::endl;
    // }
    return 0;
}
