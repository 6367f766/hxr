#ifndef TYPE_LIST_H
#define TYPE_LIST_H
#include <iostream>
#include <map>
#include <tuple>
#include <type_traits>

enum class Type : uint16_t {
    Uint32_t = 0,
    Uint64_t = 1,
    Int32_t = 2,
    Int64_t = 3,
    String = 4,
    Undefined = 0xFFFFU
};

template <Type E>
struct EnumToType;

template <>
struct EnumToType<Type::Int32_t> {
    using type = int32_t;
};

template <>
struct EnumToType<Type::Int64_t> {
    using type = int64_t;
};

template <>
struct EnumToType<Type::Uint32_t> {
    using type = uint32_t;
};

template <>
struct EnumToType<Type::Uint64_t> {
    using type = uint64_t;
};

template <>
struct EnumToType<Type::String> {
    using type = std::string;
};

template <Type E>
using EnumToType_t = typename EnumToType<E>::type;

struct MapItem {
    const char* key;
    Type value;
};

static constexpr std::array<MapItem, 5> Map = {
    MapItem{"u", Type::Uint32_t}, MapItem{"ul", Type::Uint64_t},
    MapItem{"d", Type::Int32_t}, MapItem{"dl", Type::Int64_t},
    MapItem{"t", Type::String}};

constexpr Type MapGet(const char* key) {
    for (const auto& pair : Map) {
        if (std::string_view(pair.key) == key) {
            return pair.value;
        }
    }
    return Type::Undefined;
}

#endif
