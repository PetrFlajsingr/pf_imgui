// https://github.com/bwrsandman/imgui-flame-graph
// v 1.00
//
// The MIT License(MIT)
//
// Copyright(c) 2019 Sandy Carter
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// History :
// 2019-10-22 First version

#pragma once

#include <climits>
#include <optional>
#include <algorithm>
#include <chrono>
#include <imgui.h>
#include <pf_common/math/Range.h>
#include <string>
#include <vector>

namespace ImGuiWidgetFlameGraph {

class FlameGraphSample {
 public:
  FlameGraphSample(pf::math::Range<std::chrono::microseconds> sampleTime,
                   std::string sampleCaption,
                   uint8_t sampleLevel = 0);

  FlameGraphSample &addSubSample(pf::math::Range<std::chrono::microseconds> sampleTime,
                                 std::string sampleCaption);

  void addSubSample(FlameGraphSample &&sample);
  void addSubSample(const FlameGraphSample &sample);
  [[nodiscard]] const pf::math::Range<std::chrono::microseconds> &getTime() const;
  [[nodiscard]] uint8_t getLevel() const;
  [[nodiscard]] const std::string &getCaption() const;
  [[nodiscard]] const std::vector<FlameGraphSample> &getSubSamples() const;

  [[nodiscard]] uint8_t getMaxDepth() const;

 private:
  pf::math::Range<std::chrono::microseconds> time;
  uint8_t level;
  std::string caption;
  std::vector<FlameGraphSample> subSamples;
};


IMGUI_API void PlotFlame(
    const char *label,
    const std::vector<FlameGraphSample> &samples,
    const std::optional<std::string>& overlay_text = std::nullopt,
    float scale_min = FLT_MAX,
    float scale_max = FLT_MAX,
    ImVec2 graph_size = ImVec2(0, 0));
}
