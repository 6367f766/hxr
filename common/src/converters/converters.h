#ifndef CONVERTERS_H
#define CONVERTERS_H
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <type_traits>

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

    void toHex() {
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(&valueRef_);
        ss_ << "   ";

        uint8_t tabCounter = 0;
        for (uint8_t i = 0; i < Config::size; i++) {
            ss_ << std::setfill('0') << std::setw(2) << std::hex
                << unsigned(*startAddr) << "\t\t   ";
            startAddr++;
            tabCounter++;
            if (tabCounter == 10) {
                ss_ << std::endl;
                tabCounter = 0;
            }
        }

        ss_ << std::endl;
    }

    void toBin() {
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(&valueRef_);
        uint8_t tabCounter = 0;
        for (uint8_t i = 0; i < Config::size; i++) {
            std::bitset<4> firstByte{unsigned((*startAddr >> 4) & 0xff)};
            ss_ << firstByte << " ";
            std::bitset<4> lastByte{unsigned(*startAddr & 0xff)};
            ss_ << lastByte;
            //  std::cout << " " << (byte & ~0xFF);
            ss_ << "\t";
            startAddr++;
            tabCounter++;
            if (tabCounter == 10) {
                ss_ << std::endl;
                tabCounter = 0;
            }
        }
        ss_ << std::endl;
    }

    void trackCounter(uint8_t& ref, std::ostringstream& lineRef,
                      bool isLast = false) {
        if (ref == 8) {
            ss_ << lineRef.str();
            ss_ << std::endl;
            lineRef.str("");

            if (isLast) {
                ref = 0;
                ss_ << std::endl;
            }
        }
    }

    void showString(char* data, uint32_t maxSize) {
        LOG_V() << "Showing string...";
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(data);
        uint8_t counter = 0;

        std::ostringstream line1;
        std::ostringstream line2;
        std::ostringstream line3;
        for (uint32_t i = 0; i < maxSize * sizeof(char); i++) {
            line1 << (char)*startAddr << "\t\t";

            line2 << std::setfill('0') << std::setw(2) << std::hex
                  << unsigned(*startAddr) << "\t\t";

            std::bitset<4> firstByte{unsigned((*startAddr >> 4) & 0xff)};
            std::bitset<4> lastByte{unsigned(*startAddr & 0xff)};
            line3 << firstByte << " " << lastByte << "\t";

            counter++;
            startAddr++;

            trackCounter(counter, line1);
            trackCounter(counter, line2);
            trackCounter(counter, line3, true);
        }
        ss_ << line1.str() << "\n";
        ss_ << line2.str() << "\n";
        ss_ << line3.str() << "\n";
    }

   public:
    Hexer(T& value) : valueRef_(value), Config{0} {};

    void setConfig(Config config) {
        Config::setConfig(config);
    }

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

            SizedText<100> sizedText{valueRef_.c_str()};
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
