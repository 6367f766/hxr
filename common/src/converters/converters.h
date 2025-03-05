#ifndef CONVERTERS_H
#define CONVERTERS_H
#include <cstdint>
#include <sstream>

namespace converters {

struct Metadata {

    void setShowAddress(bool value) { showAddress = value; }
    void setShowBinary(bool value) { showBinary = value; }
    void setIsString(bool value) { isString_ = value; }
    void setIsHex(bool value) { isHex_ = value; }
    void setIs64Bit(bool value) { is64bit_ = value; }
    void setIsSigned(bool value) { isSigned_ = value; }

    bool getShowAddress() { return showAddress; }
    bool getShowBinary() { return showBinary; }

    bool isString() { return isString_; }
    bool isHex() { return isHex_; }
    bool is64Bit() { return is64bit_; }
    bool isSinged() { return isSigned_; }

   private:
    bool showAddress{false};
    bool showBinary{false};
    bool isString_{false};
    bool isHex_{false};
    bool is64bit_{false};
    bool isSigned_{false};
};

template <class T, class Config = Metadata>
class Hexer : public Config {
    T& valueRef_;

    void toHex() {
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(&valueRef_);
        std::cout << "   ";

        uint8_t tabCounter = 0;
        for (uint8_t i = 0; i < sizeof(T); i++) {
            std::cout << std::setfill('0') << std::setw(2) << std::hex
                      << unsigned(*startAddr) << "\t\t   ";
            startAddr++;
            tabCounter++;
            if (tabCounter == 10) {
                std::cout << std::endl;
                tabCounter = 0;
            }
        }
        std::cout << std::endl;
    }

    void toBin() {
        uint8_t* startAddr = reinterpret_cast<uint8_t*>(&valueRef_);

        uint8_t tabCounter = 0;
        for (uint8_t i = 0; i < sizeof(T); i++) {
            std::bitset<4> firstByte{unsigned((*startAddr >> 4) & 0xff)};
            std::cout << firstByte << " ";
            std::bitset<4> lastByte{unsigned(*startAddr & 0xff)};
            std::cout << lastByte;
            //  std::cout << " " << (byte & ~0xFF);
            std::cout << "\t";
            startAddr++;
            tabCounter++;
            if (tabCounter == 10) {
                std::cout << std::endl;
                tabCounter = 0;
            }
        }
        std::cout << std::endl;
    }

    void trackCounter(uint8_t& ref, std::ostringstream& lineRef,
                      bool isLast = false) {
        //
        if (ref == 8) {
            std::cout << lineRef.str();
            std::cout << std::endl;
            lineRef.str("");

            if (isLast) {
                ref = 0;
                std::cout << std::endl;
            }
        }
    }

    void showString(char* data, uint32_t maxSize) {
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
        std::cout << line1.str();
        std::cout << std::endl;
        std::cout << line2.str();
        std::cout << std::endl;
        std::cout << line3.str();
        std::cout << std::endl;
    }

   public:
    Hexer(T& value) : valueRef_(value){};

    void run() {
        if (Config::isHex()) {
            std::cout << valueRef_ << std::endl;
            if (Config::getShowBinary()) {
                toBin();
            }
            return;
        }

        if constexpr (std::is_same_v<T, std::string>) {
            SizedText<100> sizedText{valueRef_.c_str()};
            showString(const_cast<char*>(sizedText.c_str()),
                       sizedText.getSize());
        } else {
            toHex();
            if (Config::getShowBinary()) {
                toBin();
            }
        }

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
