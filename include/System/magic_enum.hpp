
//  __  __             _        ______                          _____
// |  \/  |           (_)      |  ____|                        / ____|_     _
// | \  / | __ _  __ _ _  ___  | |__   _ __  _   _ _ __ ___     | |   _| |_ _| |_
// | |\/| |/ _` |/ _` | |/ __| |  __| | '_ \| | | | '_ ` _ \    | |  |_   _|_   _|
// | |  | | (_| | (_| | | (__  | |____| | | | |_| | | | | | |   | |____|_|   |_|
// |_|  |_|\__,_|\__, |_|\___| |______|_| |_|\__,_|_| |_| |_|    \_____|
//                __/ |                         _ _ _
//               |___/                         | (_) |
//                                             | |_| |__
//                                             | | | '_ \
//                                             | | | |_) |
//                                             |_|_|_.__/
// https://github.com/Neargye/magic_enum
// version 0.9.5
//
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2019 - 2023 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef NEARGYE_MAGIC_ENUM_HPP
#define NEARGYE_MAGIC_ENUM_HPP

#define MAGIC_ENUM_VERSION_MAJOR 0
#define MAGIC_ENUM_VERSION_MINOR 9
#define MAGIC_ENUM_VERSION_PATCH 5

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <string_view>
#include <type_traits>
#include <utility>

#if defined(__cpp_lib_format)
#include <format>
#endif

// Checks magic_enum compiler compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER) && _MSC_VER >= 1910 || defined(__RESHARPER__)
#  undef  MAGIC_ENUM_SUPPORTED
#  define MAGIC_ENUM_SUPPORTED 1
#endif

// Checks magic_enum compiler aliases compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER) && _MSC_VER >= 1920
#  undef  MAGIC_ENUM_SUPPORTED_ALIASES
#  define MAGIC_ENUM_SUPPORTED_ALIASES 1
#endif

// Enum value must be greater or equal than MAGIC_ENUM_RANGE_MIN. By default MAGIC_ENUM_RANGE_MIN = -128.
// If need another min range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MIN.
#if !defined(MAGIC_ENUM_RANGE_MIN)
#  define MAGIC_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equal than MAGIC_ENUM_RANGE_MAX. By default MAGIC_ENUM_RANGE_MAX = 128.
// If need another max range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MAX.
#if !defined(MAGIC_ENUM_RANGE_MAX)
#  define MAGIC_ENUM_RANGE_MAX 128
#endif

namespace magic_enum {

// Enum value must be in range [MAGIC_ENUM_RANGE_MIN, MAGIC_ENUM_RANGE_MAX]. By default MAGIC_ENUM_RANGE_MIN = -128, MAGIC_ENUM_RANGE_MAX = 128.
// If need another range for all enum types by default, redefine the macro MAGIC_ENUM_RANGE_MIN and MAGIC_ENUM_RANGE_MAX.
// If need another range for specific enum type, add specialization enum_range for necessary enum type.
template <typename E>
struct enum_range {
  static_assert(std::is_enum_v<E>, "magic_enum::enum_range requires enum type.");
  static constexpr int min = MAGIC_ENUM_RANGE_MIN;
  static constexpr int max = MAGIC_ENUM_RANGE_MAX;
  static_assert(max > min, "magic_enum::enum_range requires max > min.");
};

static_assert(MAGIC_ENUM_RANGE_MAX > MAGIC_ENUM_RANGE_MIN, "MAGIC_ENUM_RANGE_MAX must be greater than MAGIC_ENUM_RANGE_MIN.");

namespace detail {

template <typename T>
struct supported
#if defined(MAGIC_ENUM_SUPPORTED) && MAGIC_ENUM_SUPPORTED || defined(MAGIC_ENUM_NO_CHECK_SUPPORT)
    : std::true_type {};
#else
    : std::false_type {};
#endif

template <typename T>
inline constexpr bool is_enum_v = std::is_enum_v<T> && std::is_same_v<T, std::decay_t<T>>;

template <typename E>
constexpr auto n() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::n requires enum type.");
#if defined(__clang__) || defined(__GNUC__)
  return std::string_view{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
  return std::string_view{__FUNCSIG__};
#else
  return std::string_view{};
#endif
}

template <typename E, E V>
constexpr auto n() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::n requires enum type.");
#if defined(__clang__) || defined(__GNUC__)
  return std::string_view{__PRETTY_FUNCTION__};
#elif defined(_MSC_VER)
  return std::string_view{__FUNCSIG__};
#else
  return std::string_view{};
#endif
}

template <typename E, E V>
constexpr bool is_valid() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::is_valid requires enum type.");
  return n<E, V>().find("(anonymous namespace)") == std::string_view::npos &&
         n<E, V>().find("operator") == std::string_view::npos;
}

template <typename E, int O, bool IsFlags = false, typename U = std::underlying_type_t<E>>
constexpr E value(std::size_t i) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::value requires enum type.");
  if constexpr (IsFlags) {
    return static_cast<E>(U{1} << static_cast<U>(static_cast<int>(i) + O));
  } else {
    return static_cast<E>(static_cast<U>(static_cast<int>(i) + O));
  }
}

template <typename E, bool IsFlags = false, typename U = std::underlying_type_t<E>>
constexpr int reflected_min() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::reflected_min requires enum type.");
  if constexpr (IsFlags) {
    return 0;
  } else {
    constexpr auto lhs = enum_range<E>::min;
    constexpr auto rhs = (std::numeric_limits<U>::min)();
    if constexpr (cmp_less(rhs, lhs)) {
      return lhs;
    } else {
      return rhs;
    }
  }
}

template <typename E, bool IsFlags = false, typename U = std::underlying_type_t<E>>
constexpr int reflected_max() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::reflected_max requires enum type.");
  if constexpr (IsFlags) {
    return std::numeric_limits<U>::digits - 1;
  } else {
    constexpr auto lhs = enum_range<E>::max;
    constexpr auto rhs = (std::numeric_limits<U>::max)();
    if constexpr (cmp_less(lhs, rhs)) {
      return lhs;
    } else {
      return rhs;
    }
  }
}

template <typename E, bool IsFlags>
inline constexpr auto reflected_min_v = reflected_min<E, IsFlags>();

template <typename E, bool IsFlags>
inline constexpr auto reflected_max_v = reflected_max<E, IsFlags>();

template <std::size_t N>
constexpr std::size_t values_count(const bool (&valid)[N]) noexcept {
  auto count = std::size_t{0};
  for (std::size_t i = 0; i < N; ++i) {
    if (valid[i]) {
      ++count;
    }
  }
  return count;
}

template <typename E, bool IsFlags, int Min, std::size_t... I>
constexpr auto values(std::index_sequence<I...>) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::values requires enum type.");
  constexpr bool valid[sizeof...(I)] = {is_valid<E, value<E, Min, IsFlags>(I)>()...};
  constexpr std::size_t count = values_count(valid);

  if constexpr (count > 0) {
    E values[count] = {};
    for (std::size_t i = 0, v = 0; v < count; ++i) {
      if (valid[i]) {
        values[v++] = value<E, Min, IsFlags>(i);
      }
    }
    return to_array(values, std::make_index_sequence<count>{});
  } else {
    return std::array<E, 0>{};
  }
}

template <typename E, bool IsFlags = false>
constexpr auto values() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::values requires enum type.");
  constexpr auto min = reflected_min_v<E, IsFlags>;
  constexpr auto max = reflected_max_v<E, IsFlags>;
  constexpr auto range_size = max - min + 1;
  static_assert(range_size > 0, "magic_enum::enum_range requires valid size.");
  static_assert(range_size < (std::numeric_limits<std::uint16_t>::max)(), "magic_enum::enum_range requires valid size.");

  return values<E, IsFlags, min>(std::make_index_sequence<range_size>{});
}

template <typename E>
inline constexpr auto values_v = values<E, false>();

template <typename E>
inline constexpr auto count_v = values_v<E>.size();

template <typename E, typename U = std::underlying_type_t<E>>
constexpr bool cmp_less(E lhs, E rhs) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::cmp_less requires enum type.");
  if constexpr (std::is_signed_v<U>) {
    return static_cast<U>(lhs) < static_cast<U>(rhs);
  } else {
    return lhs < rhs;
  }
}

template <typename I>
constexpr bool cmp_less(I lhs, I rhs) noexcept {
  static_assert(std::is_integral_v<I>, "magic_enum::detail::cmp_less requires integral type.");
  if constexpr (std::is_signed_v<I>) {
    return lhs < rhs;
  } else {
    return lhs < rhs;
  }
}

template <typename T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N], std::index_sequence<I...>) noexcept {
  return {{a[I]...}};
}

template <typename E, E V>
constexpr auto enum_name() noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::enum_name requires enum type.");
  constexpr auto name = n<E, V>();
  constexpr auto prefix = std::string_view{"magic_enum::detail::n() [with E = "};
  constexpr auto suffix = std::string_view{"; V = "};
  constexpr auto suffix2 = std::string_view{"]"};
  constexpr auto prefix_size = name.find(prefix) + prefix.size();
  constexpr auto suffix_pos = name.find(suffix);
  constexpr auto name_size = suffix_pos - prefix_size;
  constexpr auto enum_name_full = name.substr(prefix_size, name_size);
  constexpr auto enum_name_size = enum_name_full.find_last_of("::");
  constexpr auto enum_name_simple = enum_name_full.substr(enum_name_size == std::string_view::npos ? 0 : enum_name_size + 1);
  
  if constexpr (enum_name_simple.empty() || enum_name_simple.front() == '(' || enum_name_simple.find("operator") != std::string_view::npos) {
    return std::string_view{};
  } else {
    return enum_name_simple;
  }
}

template <typename E, E V>
inline constexpr auto enum_name_v = enum_name<E, V>();

template <typename E, std::size_t... I>
constexpr auto enum_names(std::index_sequence<I...>) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::enum_names requires enum type.");
  constexpr auto names = std::array<std::string_view, sizeof...(I)>{{enum_name_v<E, values_v<E>[I]>...}};
  return names;
}

template <typename E>
inline constexpr auto enum_names_v = enum_names<E>(std::make_index_sequence<count_v<E>>{});

template <typename E, typename BinaryPredicate>
constexpr bool is_sorted(BinaryPredicate&& p) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::is_sorted requires enum type.");
  for (std::size_t i = 1; i < count_v<E>; ++i) {
    if (!p(values_v<E>[i - 1], values_v<E>[i])) {
      return false;
    }
  }
  return true;
}

template <typename E>
inline constexpr bool is_sorted_v = is_sorted<E>([](E lhs, E rhs) { return cmp_less(lhs, rhs); });

template <typename E, typename U = std::underlying_type_t<E>>
constexpr E enum_cast(U value) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::enum_cast requires enum type.");
  return static_cast<E>(value);
}

template <typename E, typename U = std::underlying_type_t<E>>
constexpr U enum_underlying(E value) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::enum_underlying requires enum type.");
  return static_cast<U>(value);
}

template <typename E>
constexpr std::optional<E> enum_cast(std::string_view value) noexcept {
  static_assert(is_enum_v<E>, "magic_enum::detail::enum_cast requires enum type.");
  for (std::size_t i = 0; i < count_v<E>; ++i) {
    if (enum_names_v<E>[i] == value) {
      return values_v<E>[i];
    }
  }
  return std::nullopt;
}

} // namespace magic_enum::detail

// Checks whether T is an Unscoped enumeration type.
// Provides the member constant value which is equal to true, if T is an [Unscoped enumeration](https://en.cppreference.com/w/cpp/language/enum#Unscoped_enumeration) type. Otherwise, value is equal to false.
template <typename T>
struct is_unscoped_enum : std::false_type {};

template <typename T>
requires (std::is_enum_v<T> && std::is_convertible_v<T, std::underlying_type_t<T>>)
struct is_unscoped_enum<T> : std::true_type {};

template <typename T>
inline constexpr bool is_unscoped_enum_v = is_unscoped_enum<T>::value;

// Checks whether T is an Scoped enumeration type.
// Provides the member constant value which is equal to true, if T is an [Scoped enumeration](https://en.cppreference.com/w/cpp/language/enum#Scoped_enumerations) type. Otherwise, value is equal to false.
template <typename T>
struct is_scoped_enum : std::false_type {};

template <typename T>
requires (std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>)
struct is_scoped_enum<T> : std::true_type {};

template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

// If T is a complete enumeration type, provides a member typedef type that names the underlying type of T.
// Otherwise, if T is not an enumeration type, there is no member type.
// Otherwise (T is an incomplete enumeration type), the program is ill-formed.
template <typename T>
using underlying_type_t = std::underlying_type_t<T>;

// Returns type name of type E.
template <typename E>
[[nodiscard]] constexpr auto enum_type_name() noexcept -> std::string_view {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_type_name requires enum type.");
  constexpr auto name = detail::n<E>();
  constexpr auto prefix = std::string_view{"magic_enum::detail::n() [with E = "};
  constexpr auto suffix = std::string_view{"]"};
  constexpr auto prefix_size = name.find(prefix) + prefix.size();
  constexpr auto suffix_pos = name.find(suffix);
  constexpr auto size = suffix_pos - prefix_size;

  return name.substr(prefix_size, size);
}

// Returns number of enum values.
template <typename E>
[[nodiscard]] constexpr auto enum_count() noexcept -> std::size_t {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_count requires enum type.");
  return detail::count_v<E>;
}

// Returns enum value at specified index.
// No bounds checking is performed: the behavior is undefined if index >= number of enum values.
template <typename E>
[[nodiscard]] constexpr auto enum_value(std::size_t index) noexcept -> E {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_value requires enum type.");
  if constexpr (detail::is_sorted_v<E>) {
    constexpr bool is_default_range = (enum_range<E>::min == MAGIC_ENUM_RANGE_MIN) && (enum_range<E>::max == MAGIC_ENUM_RANGE_MAX);
    constexpr bool is_pow2_range = (enum_range<E>::min == 0) && ((enum_range<E>::max + 1) & enum_range<E>::max) == 0;
    if constexpr (is_default_range || is_pow2_range) {
      return static_cast<E>(static_cast<underlying_type_t<E>>(index) + enum_range<E>::min);
    }
  }

  return detail::values_v<E>[index];
}

// Returns std::array with enum values, sorted by enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_values() noexcept -> std::array<E, enum_count<E>()> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_values requires enum type.");
  return detail::values_v<E>;
}

// Returns name from enum value.
// If enum value does not have name or value out of range, returns empty string.
template <typename E>
[[nodiscard]] constexpr auto enum_name(E value) noexcept -> std::string_view {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_name requires enum type.");
  if constexpr (detail::count_v<E> == 0) {
    static_cast<void>(value);
    return {};
  } else {
    if constexpr (detail::is_sorted_v<E>) {
      constexpr bool is_default_range = (enum_range<E>::min == MAGIC_ENUM_RANGE_MIN) && (enum_range<E>::max == MAGIC_ENUM_RANGE_MAX);
      constexpr bool is_pow2_range = (enum_range<E>::min == 0) && ((enum_range<E>::max + 1) & enum_range<E>::max) == 0;
      if constexpr (is_default_range || is_pow2_range) {
        const auto index = static_cast<std::size_t>(static_cast<underlying_type_t<E>>(value) - enum_range<E>::min);
        if (index < detail::count_v<E>) {
          return detail::enum_names_v<E>[index];
        }
      }
    }

    for (std::size_t i = 0; i < detail::count_v<E>; ++i) {
      if (enum_value<E>(i) == value) {
        return detail::enum_names_v<E>[i];
      }
    }

    return {};
  }
}

// Returns std::array with names, sorted by enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_names() noexcept -> std::array<std::string_view, enum_count<E>()> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_names requires enum type.");
  return detail::enum_names_v<E>;
}

// Returns enum value from name.
// If name does not have enum value, returns std::nullopt.
template <typename E>
[[nodiscard]] constexpr auto enum_cast(std::string_view value) noexcept -> std::optional<E> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_cast requires enum type.");
  return detail::enum_cast<E>(value);
}

// Returns enum value from integer.
template <typename E>
[[nodiscard]] constexpr auto enum_cast(underlying_type_t<E> value) noexcept -> std::optional<E> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_cast requires enum type.");
  if constexpr (detail::count_v<E> == 0) {
    static_cast<void>(value);
    return std::nullopt;
  } else {
    if constexpr (detail::is_sorted_v<E>) {
      constexpr bool is_default_range = (enum_range<E>::min == MAGIC_ENUM_RANGE_MIN) && (enum_range<E>::max == MAGIC_ENUM_RANGE_MAX);
      constexpr bool is_pow2_range = (enum_range<E>::min == 0) && ((enum_range<E>::max + 1) & enum_range<E>::max) == 0;
      if constexpr (is_default_range || is_pow2_range) {
        const auto v = static_cast<E>(value);
        const auto index = static_cast<std::size_t>(value - enum_range<E>::min);
        if (index < detail::count_v<E> && enum_value<E>(index) == v) {
          return v;
        }
      }
    }

    const auto v = static_cast<E>(value);
    for (std::size_t i = 0; i < detail::count_v<E>; ++i) {
      if (enum_value<E>(i) == v) {
        return v;
      }
    }

    return std::nullopt;
  }
}

// Returns integer value from enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_integer(E value) noexcept -> underlying_type_t<E> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_integer requires enum type.");
  return static_cast<underlying_type_t<E>>(value);
}

// Returns underlying integer value from enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_underlying(E value) noexcept -> underlying_type_t<E> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_underlying requires enum type.");
  return static_cast<underlying_type_t<E>>(value);
}

// Returns index in enum values from enum value.
// If enum value does not have index, returns std::nullopt.
template <typename E>
[[nodiscard]] constexpr auto enum_index(E value) noexcept -> std::optional<std::size_t> {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_index requires enum type.");
  if constexpr (detail::count_v<E> == 0) {
    static_cast<void>(value);
    return std::nullopt;
  } else {
    if constexpr (detail::is_sorted_v<E>) {
      constexpr bool is_default_range = (enum_range<E>::min == MAGIC_ENUM_RANGE_MIN) && (enum_range<E>::max == MAGIC_ENUM_RANGE_MAX);
      constexpr bool is_pow2_range = (enum_range<E>::min == 0) && ((enum_range<E>::max + 1) & enum_range<E>::max) == 0;
      if constexpr (is_default_range || is_pow2_range) {
        const auto index = static_cast<std::size_t>(static_cast<underlying_type_t<E>>(value) - enum_range<E>::min);
        if (index < detail::count_v<E> && enum_value<E>(index) == value) {
          return index;
        }
      }
    }

    for (std::size_t i = 0; i < detail::count_v<E>; ++i) {
      if (enum_value<E>(i) == value) {
        return i;
      }
    }

    return std::nullopt;
  }
}

// Returns whether enum contains enumerator with such enum value.
template <typename E>
[[nodiscard]] constexpr auto enum_contains(E value) noexcept -> bool {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_contains requires enum type.");
  return enum_cast<E>(static_cast<underlying_type_t<E>>(value)).has_value();
}

// Returns whether enum contains enumerator with such integer value.
template <typename E>
[[nodiscard]] constexpr auto enum_contains(underlying_type_t<E> value) noexcept -> bool {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_contains requires enum type.");
  return enum_cast<E>(value).has_value();
}

// Returns whether enum contains enumerator with such name.
template <typename E>
[[nodiscard]] constexpr auto enum_contains(std::string_view value) noexcept -> bool {
  static_assert(detail::is_enum_v<E>, "magic_enum::enum_contains requires enum type.");
  return enum_cast<E>(value).has_value();
}

} // namespace magic_enum

#endif // NEARGYE_MAGIC_ENUM_HPP
