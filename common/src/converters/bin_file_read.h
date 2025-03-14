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
    converters::ArgumentMetadata metadata_{0};
    bool showBinary_;

    auto read_binary_as_char() -> void {
        LOG_V() << "reading binary: " << filename_ << " as char";
        std::ifstream file(filename_.data());
        char character;
        std::vector<char> data_{};

        uint32_t counter = 0;
        char tmp;
        converters::Hexer<char> hxr{tmp};
        hxr.setConfig(metadata_);

        while (file.get(character)) {
            if (static_cast<uint8_t>(character) < 0x20) {
                data_.push_back('.');
            } else {
                data_.push_back(character);
            }

            counter++;
            if (counter == 4096) {
                hxr.showString(data_.data(), data_.size());
                hxr.log();
                data_ = {};
                counter = 0;
            }
        }
        file.close();
        hxr.showString(data_.data(), data_.size());
        hxr.log();
    }

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
                converters::Hexer<T> hxr{value};
                hxr.setConfig(metadata_);
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
        converters::Hexer<T> hxr{value};
        hxr.setConfig(metadata_);
        hxr.toBin(" ");
        hxr.log();
        /// XXX: would be nice to not use std::cout here
        std::cout << value;
        file.close();
    }

   public:
    explicit BinFileRead(const std::string_view filename, bool showBinary)
        : filename_{filename}, showBinary_{showBinary} {};

    template <class T>
    void run() {
        LOG_V() << "Show binary: " << showBinary_;
        metadata_ = converters::ArgumentMetadata{sizeof(T)}
                        .withShowBinary(showBinary_)
                        .withSize(sizeof(T))
                        .withArgumentType<T>();

        if constexpr (std::is_same_v<T, char>) {
            read_binary_as_char();
        } else {
            read_binary<T>();
        }
    }
};
#endif
