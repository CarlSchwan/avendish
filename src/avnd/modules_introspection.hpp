#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <avnd/concepts/modules.hpp>
#include <avnd/effect_container.hpp>
#include <common/struct_reflection.hpp>

namespace avnd
{
template <typename T>
using modules_introspection
    = fields_introspection<typename modules_type<T>::type>;

template <typename T>
auto& get_modules(avnd::effect_container<T>& t)
{
  return t.modules();
}
template <avnd::modules_is_type T>
auto& get_modules(T& t)
{
  static_assert(std::is_void_v<T>, "Cannot get modules on T");
}
template <avnd::modules_is_value T>
auto& get_modules(T& t)
{
  return t.modules;
}

template <typename T>
static constexpr void for_all_modules(T& obj, auto&& func) noexcept
{
  if constexpr (modules_introspection<T>::size > 0)
  {
    boost::pfr::for_each_field(get_modules(obj), func);
  }
}
}
