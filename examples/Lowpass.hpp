#pragma once

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <vector>

namespace examples
{
/**
 * Basic example of low-pass filter
 */
struct Lowpass
{
  static consteval auto name() { return "Lowpass"; }
  static consteval auto c_name() { return "avnd_lowpass"; }
  static consteval auto uuid()
  {
    return "64475459-0116-4f9d-8d5a-b8f054b00fa5";
  }

  struct
  {
    struct
    {
      // Same for the inlet / outlet metadata.
      static consteval auto name() { return "Input"; }
      double** samples;
      int channels;
    } audio;

    struct
    {
      static consteval auto name() { return "Weight"; }
      static consteval auto control()
      {
        struct
        {
          const float min = 0.;
          const float max = 1.;
          const float init = 0.5;
        } c;
        return c;
      }

      float value;
    } weight;
  } inputs;

  struct
  {
    struct
    {
      static consteval auto name() { return "Output"; }
      double** samples;
      int channels;
    } audio;
  } outputs;

  // Here we have some state which depends on the host configuration (number of channels, etc).
  std::vector<float> previous_values{};

  struct setup
  {
    int channels{};
    float rate{};
  };

  // One can add such a method to recompute internal state, buffers...
  // It will be called whenever any of the values here change.
  void prepare(setup info)
  {
    // Note that the host will detect which members are available in
    // your setup struct: objects which only need channels may pass a
    // struct with only that member as argument.
    previous_values.resize(info.channels);
  }

  // Do our processing for N samples
  void operator()(int N)
  {
    const auto weight = inputs.weight.value;
    auto& p1 = inputs.audio;
    auto& p2 = outputs.audio;

    const auto chans = p1.channels;

    // Process the input buffer
    for (int i = 0; i < chans; i++)
    {
      auto& in = p1.samples[i];
      auto& out = p2.samples[i];

      float& prev = this->previous_values[i];

      for (int j = 0; j < N; j++)
      {
        out[j] = weight * in[j] + (1.0 - weight) * prev;
        prev = out[j];
      }
    }
  }
};
}
