#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later OR BSL-1.0 OR CC0-1.0 OR CC-PDCC OR 0BSD */

#include <boost/pfr.hpp>

#include <type_traits>

namespace avnd
{
struct dummy
{
};

#define if_possible(action)           \
  if constexpr (requires { action; }) \
  {                                   \
    action;                           \
  }

#define value_if_possible(A, X, B) \
  []() consteval                   \
  {                                \
    if constexpr (requires { A; }) \
      return A;                    \
    else                           \
      return B;                    \
  }                                \
  ()

// Very generic concepts
template <typename T>
concept vector_ish = requires(T t)
{
  t.push_back({});
  t.size();
  t.reserve(1);
  t.resize(1);
  t.clear();
  t[0];
};

template <typename T>
concept span_ish = requires(T t)
{
  t.size();
  t[0];
};

template <typename T>
concept pointer = std::is_pointer_v<std::remove_reference_t<T>>;

template <typename T>
concept string_ish = requires(T t, std::string_view v)
{
  v = t;
};

template <typename T>
struct is_type_list : std::false_type
{
};
template <template <typename...> typename T, typename... Args>
struct is_type_list<T<Args...>> : std::true_type
{
};

template <typename T>
concept type_list = is_type_list<std::remove_const_t<T>>::value;

template <typename T>
concept enum_ish = std::is_enum_v<std::decay<T>>;

/**
 * A macro to define a set of concepts & type-checks to devise if
 * in the expression T::foo, foo is a type or a value.
 *
 * Will define:
 * concept foo_is_value;
 * concept foo_is_type;
 * concept has_foo;
 *
 *
 * Used for instance for inputs and outputs introspection.
 * C++2y: we'd like to have the token "inputs" itself be generic
 */
#define type_or_value_qualification(Name) \
  template <typename T>                   \
  concept Name##_is_value = requires(T t) \
  {                                       \
    decltype(T::Name){};                  \
  };                                      \
                                          \
  template <typename T>                   \
  concept Name##_is_type = requires(T t)  \
  {                                       \
    !std::is_void_v<typename T::Name>;    \
  };                                      \
                                          \
  template <typename T>                   \
  concept has_##Name = Name##_is_type<T> || Name##_is_value<T>;

/**
 * A macro that leverages the previous macro to define helper types,
 * which will allow to get access to the information of a named field
 * no matter if it's a type or a value.
 *
 * e.g. the following types allow to get the type of the
 * "inputs" / "outputs" member whether they are types
 * or values:
 *
 * struct foo {
 *   struct inputs {
 *     ..
 *   };
 * };
 *
 * struct bar {
 *   struct {
 *     ..
 *   } inputs;
 * };
 *
 * inputs_type<foo>::type will yield the type of "inputs".
 * inputs_type<bar>::type will yield the type of "inputs".
 *
 */
#define type_or_value_reflection(Name)                                      \
  template <typename T>                                                     \
  struct Name##_type                                                        \
  {                                                                         \
    using type = dummy;                                                     \
    using tuple = std::tuple<>;                                             \
  };                                                                        \
                                                                            \
  template <Name##_is_type T>                                               \
  struct Name##_type<T>                                                     \
  {                                                                         \
    using type = typename T::Name;                                          \
    using tuple = decltype(boost::pfr::structure_to_tuple(type{}));         \
  };                                                                        \
                                                                            \
  template <Name##_is_value T>                                              \
  struct Name##_type<T>                                                     \
  {                                                                         \
    using type = std::remove_reference_t<decltype(std::declval<T>().Name)>; \
    using tuple = decltype(boost::pfr::structure_to_tuple(type{}));         \
  };
}
