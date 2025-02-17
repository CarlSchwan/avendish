#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <algorithm>

#include <string_view>

namespace avnd
{

template <std::size_t N>
struct limited_string_view : std::string_view
{
  using std::string_view::string_view;
  template <std::size_t M>
  constexpr limited_string_view(const char (&str)[M])
      : std::string_view{str, M}
  {
    static_assert(M < N, "A name is too long");
  }
  constexpr limited_string_view(std::string_view str)
      : std::string_view{str.data(), std::min(str.size(), N)}
  {
  }

  void copy_to(void* dest) const noexcept
  {
    std::copy_n(data(), size(), reinterpret_cast<char*>(dest));
  }
};

}
