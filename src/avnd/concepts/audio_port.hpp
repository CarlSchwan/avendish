#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later OR BSL-1.0 OR CC0-1.0 OR CC-PDCC OR 0BSD */

#include <avnd/concepts/generic.hpp>
#include <common/concepts_polyfill.hpp>

namespace avnd
{

// struct { float sample; };
template <typename FP, typename T>
concept audio_sample_port = std::same_as<decltype(T::sample), FP>;
template <typename T>
concept generic_audio_sample_port = std::floating_point<decltype(T::sample)>;

template <typename FP, typename T>
using is_audio_sample_port = std::
    conditional_t<audio_sample_port<FP, T>, std::true_type, std::false_type>;

template <typename FP>
struct is_audio_sample_port_q
{
  template <typename T>
  using fn = std::
      conditional_t<audio_sample_port<FP, T>, std::true_type, std::false_type>;
};

// struct { float* sample; };
template <typename FP, typename T>
concept mono_array_sample_port = std::same_as<decltype(T::samples), FP*>;

template <typename FP, typename T>
using is_mono_array_sample_port = std::conditional_t<
    mono_array_sample_port<FP, T>,
    std::true_type,
    std::false_type>;

template <typename FP>
struct is_mono_array_sample_port_q
{
  template <typename T>
  using fn = std::conditional_t<
      mono_array_sample_port<FP, T>,
      std::true_type,
      std::false_type>;
};

// struct { float** sample; };
template <typename FP, typename T>
concept poly_array_sample_port = std::same_as<decltype(T::samples), FP**>;

template <typename FP, typename T>
using is_poly_array_sample_port = std::conditional_t<
    poly_array_sample_port<FP, T>,
    std::true_type,
    std::false_type>;

template <typename FP>
struct is_poly_array_sample_port_q
{
  template <typename T>
  using fn = std::conditional_t<
      poly_array_sample_port<FP, T>,
      std::true_type,
      std::false_type>;
};

template <typename T>
concept audio_channel = audio_sample_port<float, T> || audio_sample_port<
    double,
    T> || mono_array_sample_port<float, T> || mono_array_sample_port<double, T>;
template <typename T>
concept audio_bus
    = poly_array_sample_port<float, T> || poly_array_sample_port<double, T>;

}
