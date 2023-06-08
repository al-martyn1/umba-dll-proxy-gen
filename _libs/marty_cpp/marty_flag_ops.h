#pragma once

#include <type_traits>
//----------------------------------------------------------------------------

// https://stackoverflow.com/questions/1448396/how-to-use-enums-as-flags-in-c

#define MARTY_CPP_MAKE_ENUM_FLAGS(TEnum)                                                       \
    inline constexpr bool operator==(TEnum a, std::underlying_type<TEnum>::type b)             \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TUnder>(a) == b;                                                    \
    }                                                                                          \
    inline constexpr bool operator==(std::underlying_type<TEnum>::type a, TEnum b)             \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return a == static_cast<TUnder>(b);                                                    \
    }                                                                                          \
    inline constexpr bool operator!=(TEnum a, std::underlying_type<TEnum>::type b)             \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TUnder>(a) != b;                                                    \
    }                                                                                          \
    inline constexpr bool operator!=(std::underlying_type<TEnum>::type a, TEnum b)             \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return a != static_cast<TUnder>(b);                                                    \
    }                                                                                          \
    inline constexpr TEnum operator<<(TEnum a, unsigned b)                                     \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TEnum>(static_cast<TUnder>(a) << b);                                \
    }                                                                                          \
    inline constexpr TEnum operator>>(TEnum a, unsigned b)                                     \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TEnum>(static_cast<TUnder>(a) >> b);                                \
    }                                                                                          \
    inline constexpr TEnum operator~(TEnum a)                                                  \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TEnum>(~static_cast<TUnder>(a));                                    \
    }                                                                                          \
    inline constexpr TEnum operator|(TEnum a, TEnum b)                                         \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TEnum>(static_cast<TUnder>(a) | static_cast<TUnder>(b));            \
    }                                                                                          \
    inline constexpr TEnum operator&(TEnum a, TEnum b)                                         \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TEnum>(static_cast<TUnder>(a) & static_cast<TUnder>(b));            \
    }                                                                                          \
    inline constexpr TEnum operator^(TEnum a, TEnum b)                                         \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        return static_cast<TEnum>(static_cast<TUnder>(a) ^ static_cast<TUnder>(b));            \
    }                                                                                          \
    inline constexpr TEnum& operator<<=(TEnum& a, unsigned b)                                  \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        a = static_cast<TEnum>(static_cast<TUnder>(a) << b );                                  \
        return a;                                                                              \
    }                                                                                          \
    inline constexpr TEnum& operator>>=(TEnum& a, unsigned b)                                  \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        a = static_cast<TEnum>(static_cast<TUnder>(a) >> b );                                  \
        return a;                                                                              \
    }                                                                                          \
    inline constexpr TEnum& operator|=(TEnum& a, TEnum b)                                      \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        a = static_cast<TEnum>(static_cast<TUnder>(a) | static_cast<TUnder>(b));               \
        return a;                                                                              \
    }                                                                                          \
    inline constexpr TEnum& operator&=(TEnum& a, TEnum b)                                      \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        a = static_cast<TEnum>(static_cast<TUnder>(a) & static_cast<TUnder>(b));               \
        return a;                                                                              \
    }                                                                                          \
    inline constexpr TEnum& operator^=(TEnum& a, TEnum b)                                      \
    {                                                                                          \
        using TUnder = typename std::underlying_type<TEnum>::type;                             \
        a = static_cast<TEnum>(static_cast<TUnder>(a) ^ static_cast<TUnder>(b));               \
        return a;                                                                              \
    }

