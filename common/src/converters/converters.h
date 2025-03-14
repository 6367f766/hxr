#ifndef CONVERTERS_H
#define CONVERTERS_H
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <type_traits>

#include "colourprint.h"
#include "common.h"
#include "logger.h"

namespace converters {

/// @class ArgumentMetadata
/// @brief Contains information on arguments
/// `size` will contain the following bit options: TODO: maybe make this an
/// enum
///
///  8
///  16
///  32
///  64
///
// This is a do it all, no real sense struct... I've sort of put all this
// together... in one place but there are no railings here...
// do be careful here be dragons.
struct ArgumentMetadata {
    ArgumentMetadata(size_t s) : size{s} {}

    ArgumentMetadata& withSize(size_t newSize) {
        size = newSize;
        return *this;
    }

    ArgumentMetadata& withShowBinary(bool value) {
        showBinary = value;
        return *this;
    }

    ArgumentMetadata& withIsString(bool value) {
        isString = value;
        return *this;
    }

    ArgumentMetadata& withIsHex(bool value) {
        isHex = value;
        return *this;
    }

    ArgumentMetadata& withIsSigned(bool value) {
        isSigned = value;
        return *this;
    }

    template <class T>
    ArgumentMetadata& withArgumentType() {
        isSigned = std::is_signed_v<T>;
        isFloating = std::is_floating_point_v<T>;
        return *this;
    }

    void setConfig(ArgumentMetadata newArg) { *this = newArg; }

    bool isString{false};
    bool isHex{false};
    bool showBinary{false};
    bool isSigned{false};
    bool isFloating{false};
    size_t size;
};

template <class T, class Config = ArgumentMetadata>
class Hexer : public Config {
    T& valueRef_;
    std::ostringstream ss_;

    bool multilineOutputSplit(SentenceGenerator& sentence, uint32_t n = 16) {
        auto sentence_output = sentence.getNext(n, true);
        if (sentence_output.has_value()) {
            ss_ << sentence_output.value();
            return true;
        }
        return false;
    }

   public:
    void toHex() {
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(&valueRef_);
        auto sentence = SentenceGenerator{Word{"\t\t"}.withPostfix("\n")};

        uint8_t tabCounter = 0;
        for (uint8_t i = 0; i < Config::size; i++) {
            {
                // XXX: would be nice to simplify this
                std::ostringstream tmp;
                tmp << std::setfill('0') << std::setw(2) << std::hex
                    << unsigned(*startAddr);
                sentence.add(Word{tmp.str()});
            }
            startAddr++;
            tabCounter++;
            if (tabCounter == 10) {
                /// XXX: need to absorb counting tabs into sentenceGenerator
                sentence.add(Word{"\n"});
                tabCounter = 0;
            }
        }
        ss_ << sentence.get();
    }

    void toBin(std::string_view newline = "\n") {
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(&valueRef_);
        uint8_t tabCounter = 0;
        auto sentence = SentenceGenerator{Word{"\t"}.withPostfix("\n")};

        for (uint8_t i = 0; i < Config::size; i++) {
            std::ostringstream tmp;
            // Remnants of byte splitting, if needed see blame for this comment
            std::bitset<8> byte{unsigned(*startAddr)};
            tmp << byte;
            sentence.add(Word{tmp.str()});

            startAddr++;
            tabCounter++;
            if (tabCounter == 10) {
                sentence.add(Word{"\n"});
                tabCounter = 0;
            }
        }
        // XXX: Note newline = " " was used show bytes in same line before.
        // rethink this...
        // ss_ << newline;
        ss_ << sentence.get();
    }

    void showString(char* data, uint32_t maxSize,
                    std::string_view newline = "\n") {
        ss_.str("");
        LOG_V() << "Showing string...";
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(data);
        uint8_t counter = 0;

        auto setenceAsText =
            SentenceGenerator{Word{""}.withPostfix("\n").withPrefix("\t|")};
        auto sentenceAsHex = SentenceGenerator{Word{" "}};
        auto sentenceAsBits = SentenceGenerator{Word{" "}};

        LOG_V() << "Max size: " << maxSize;
        for (uint32_t i = 0; i < maxSize; i++) {
            setenceAsText.add(Word{std::string{(char)*startAddr}});

            {
                // XXX: would be nice to simplify this
                std::ostringstream tmp;
                tmp << std::setfill('0') << std::setw(2) << std::hex
                    << unsigned(*startAddr);
                sentenceAsHex.add(Word{tmp.str()});
            }

            std::ostringstream tmp;
            std::bitset<8> byte{unsigned(*startAddr)};
            tmp << byte;
            sentenceAsBits.add(Word{tmp.str()});

            counter++;
            startAddr++;
        }

        if (Config::showBinary) {
            bool res = multilineOutputSplit(sentenceAsBits);
            while (res) {
                multilineOutputSplit(setenceAsText);
                res = multilineOutputSplit(sentenceAsBits);
            };
        } else {
            bool res = multilineOutputSplit(sentenceAsHex, 32);
            while (res) {
                multilineOutputSplit(setenceAsText, 32);
                res = multilineOutputSplit(sentenceAsHex, 32);
            };
        }
    }

    Hexer(T& value) : valueRef_(value), Config{0} {};

    void setConfig(Config config) { Config::setConfig(config); }

    void run() {
        LOG_V() << "Running main...";
        if (Config::isHex) {
            LOG_V() << "Value is hex";
            ss_ << valueRef_ << std::endl;
            if (Config::showBinary) {
                LOG_V() << "Show binary on...";
                toBin();
            }
            LOG_B() << ss_.str();
            return;
        }

        if constexpr (std::is_same_v<T, std::string>) {
            LOG_V() << "Value is text";

            SizedText<250> sizedText{valueRef_.c_str()};
            showString(const_cast<char*>(sizedText.c_str()),
                       sizedText.getSize());
        } else {
            toHex();
            if (Config::showBinary) {
                LOG_V() << "Show binary on...";
                toBin();
            }
        }
        LOG_B() << ss_.str();
    }

    void log() { LOG_B() << ss_.str(); }
};

template <class T>
void toHex(T& value) {
    uint8_t* startAddr = reinterpret_cast<uint8_t*>(&value);
    for (uint8_t i = 0; i < sizeof(T); i++) {
        std::cout << std::setfill('0') << std::setw(2) << std::hex
                  << unsigned(*startAddr) << " ";
        startAddr++;
    }
    std::cout << std::endl;
}

void inplace_swap(int* x, int* y) {
    *y = *x ^ *y;
    toHex<int>(*y);

    *x = *x ^ *y;
    toHex<int>(*x);

    *y = *x ^ *y;
    toHex<int>(*y);
}

#define PRINT_HEX_OF(x, t)                                          \
    {                                                               \
        std::cout << "Hex for: \"" << #x << "\""                    \
                  << " of type (" << #t << ") size = " << sizeof(t) \
                  << " address: " << &x << " --> ";                 \
        converters::toHex<t>(x);                                    \
    }

}  // namespace converters
#endif
