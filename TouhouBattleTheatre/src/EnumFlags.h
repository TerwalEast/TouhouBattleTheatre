#pragma once

#include <type_traits>

template<typename E>
struct enable_bitmask_operators : std::false_type {};

#define ENABLE_BITMASK_OPERATORS(E) \
template<> \
struct enable_bitmask_operators<E> : std::true_type {};

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::value, E>
operator|(E lhs, E rhs)
{
    using underlying = typename std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::value, E>
operator&(E lhs, E rhs)
{
    using underlying = typename std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::value, E&>
operator|=(E& lhs, E rhs)
{
    using underlying = typename std::underlying_type_t<E>;
    lhs = static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
    return lhs;
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::value, E&>
operator&=(E& lhs, E rhs)
{
    using underlying = typename std::underlying_type_t<E>;
    lhs = static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
    return lhs;
}

template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::value, E>
operator~(E val)
{
    using underlying = typename std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<underlying>(val));
}

// 辅助函数，用于检查是否设置了某个标志
template<typename E>
typename std::enable_if_t<enable_bitmask_operators<E>::value, bool>
HasFlag(E flags, E flag)
{
    return (flags & flag) != static_cast<E>(0);
}