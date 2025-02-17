#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

// See Helpers.hpp for a better example
#if AVND_PUREDATA
#include <m_pd.h>
#elif AVND_MAXMSP
#include <ext.h>
#else
#include <cstdio>
#define post printf
#endif

namespace examples
{
static void free_function(float a)
{
  post("free_function: %f", a);
}

/**
 * This example shows handling of messages in message-oriented
 * environments such as Pd / Max
 */
struct Messages
{
  static consteval auto name() { return "Messages"; }
  static consteval auto c_name() { return "avnd_messages"; }
  static consteval auto uuid()
  {
    return "c1995e22-22a8-4149-8330-89d8a99850c4";
  }

  struct
  {
    struct
    {
      float value;
    } a;
    struct
    {
      float value;
    } b;
  } inputs;

  struct
  {
    struct
    {
      float value;
    } out;
  } outputs;

  struct
  {
    struct
    {
      static consteval auto name() { return "member"; }
      static consteval auto func() { return &Messages::bamboozle; }
    } member;
#if (defined(__clang__) || defined(_MSC_VER))
    // broken in GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83258
    struct
    {
      static consteval auto name() { return "lambda_function"; }
      static consteval auto func()
      {
        return +[] { post("lambda"); };
      }
    } lambda;
#endif
    struct
    {
      static consteval auto name() { return "function"; }
      static consteval auto func() { return free_function; }
    } freefunc;
  } messages;

  void bamboozle(float x, float y, const char* str)
  {
    inputs.a.value = x;
    inputs.b.value = y;

    post("bamboozle: %f %f %s", x, y, str);
  }

  void operator()() { outputs.out.value = inputs.a.value + inputs.b.value; }
};
}
