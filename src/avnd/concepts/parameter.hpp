#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later OR BSL-1.0 OR CC0-1.0 OR CC-PDCC OR 0BSD */

#include <common/concepts_polyfill.hpp>

#include <string>

namespace avnd
{
/** A "parameter" port is something that has a value:
 * struct {
 *   float value;
 * };
 */

// Concepts related to inputs / outputs
template <typename T>
concept parameter = requires(T t)
{
  {
    t.value
    } -> std::default_initializable;
};

template <typename T>
concept int_ish = std::is_same_v < std::remove_reference_t<T>,
signed int >
    || std::is_same_v<std::remove_reference_t<T>, signed short> || std::is_same_v<std::remove_reference_t<T>, signed long> || std::is_same_v<std::remove_reference_t<T>, signed long long> || std::is_same_v<std::remove_reference_t<T>, unsigned int> || std::is_same_v<std::remove_reference_t<T>, unsigned short> || std::is_same_v<std::remove_reference_t<T>, unsigned long> || std::is_same_v<std::remove_reference_t<T>, unsigned long long>;

template <typename T>
concept fp_ish = std::is_floating_point_v<std::remove_reference_t<T>>;

template <typename T>
concept bool_ish = std::is_same_v < std::remove_reference_t<T>,
bool > ;

template <typename T>
concept int_parameter = requires(T t)
{
  {
    t.value
    } -> int_ish;
};

template <typename T>
concept float_parameter = requires(T t)
{
  {
    t.value
    } -> fp_ish;
};

template <typename T>
concept bool_parameter = requires(T t)
{
  {
    t.value
    } -> bool_ish;
};

template <typename T>
concept string_parameter = requires(T t)
{
  {
    t.value
    } -> std::convertible_to<std::string>;
};

template <typename T>
concept parameter_with_range = requires
{
  T::control().min;
  T::control().max;
  T::control().init;
};

template <typename T>
concept enum_parameter = std::is_enum_v<decltype(T::value)>;

}
