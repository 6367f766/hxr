#ifndef BIN_FILE_READ_H
#define BIN_FILE_READ_H
#include <array>
#include <cmath>
#include <cstring>
#include <string_view>

#include "converters.h"
#include "logger.h"

class BinFileRead {
    std::string_view filename_;

    template <class T>
    auto read_binary() -> void {
        LOG_V() << "reading binary for " << filename_;

        std::ifstream file(filename_.data());
        char character;
        T value;
        uint32_t counter = 0;
        std::array<char, sizeof(T)> charvec_{};

        while (file.get(character)) {
            if (counter == sizeof(T)) {
                counter = 0;
                std::memcpy(&value, charvec_.data(), sizeof(T));
                auto metadata = converters::ArgumentMetadata{sizeof(T)}
                                    .withShowBinary(true)
                                    .withArgumentType<T>();
                converters::Hexer<T> hxr{value};
                hxr.setConfig(metadata);
                hxr.toBin(" ");
                hxr.log();
                /// XXX: would be nice to not use std::cout here
                std::cout << value;

                charvec_.fill(0);
                charvec_[counter] = character;
                LOG_V() << "Pushing: " << unsigned(character) << " to buffer";
            } else {
                charvec_[counter] = character;
                LOG_V() << "Pushing: " << unsigned(character) << " to buffer";
            }
            counter++;
        }
        std::memcpy(&value, charvec_.data(), sizeof(T));
        auto metadata = converters::ArgumentMetadata{sizeof(T)}
                            .withShowBinary(true)
                            .withArgumentType<T>();
        converters::Hexer<T> hxr{value};
        hxr.setConfig(metadata);
        hxr.toBin(" ");
        hxr.log();
        /// XXX: would be nice to not use std::cout here
        std::cout << value;
        file.close();
    }

   public:
    explicit BinFileRead(const std::string_view filename)
        : filename_{filename} {};

    template <class T>
    void run() {
        read_binary<T>();
    }
};
#endif
