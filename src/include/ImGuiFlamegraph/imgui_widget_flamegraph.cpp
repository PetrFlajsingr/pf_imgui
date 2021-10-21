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

#include <imgui_widget_flamegraph.h>
#include <functional>

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

namespace ImGuiWidgetFlameGraph {
FlameGraphSample::FlameGraphSample(pf::math::Range<std::chrono::microseconds> sampleTime, std::string sampleCaption, uint8_t sampleLevel)
    : time(sampleTime), level(sampleLevel), caption(std::move(sampleCaption)) {}
FlameGraphSample &FlameGraphSample::addSubSample(pf::math::Range<std::chrono::microseconds> sampleTime, std::string sampleCaption) {
  assert(time.start <= sampleTime.start);
  assert(sampleTime.end <= time.end);
  subSamples.emplace_back(sampleTime, std::move(sampleCaption), level + 1);
  return subSamples.back();
}
const pf::math::Range<std::chrono::microseconds> &FlameGraphSample::getTime() const {
  return time;
}
uint8_t FlameGraphSample::getLevel() const {
  return level;
}
const std::string &FlameGraphSample::getCaption() const {
  return caption;
}
const std::vector<FlameGraphSample> &FlameGraphSample::getSubSamples() const {
  return subSamples;
}
uint8_t FlameGraphSample::getMaxDepth() const {
  if (subSamples.empty()) {
    return level;
  }
  return std::ranges::max_element(subSamples, [](const auto &a, const auto &b) {
    return a.getMaxDepth() < b.getMaxDepth();
  })
      ->getMaxDepth();
}
void FlameGraphSample::addSubSample(FlameGraphSample &&sample) {
  subSamples.emplace_back(std::forward<FlameGraphSample>(sample));
}
void FlameGraphSample::addSubSample(const FlameGraphSample &sample) {
  subSamples.emplace_back(sample);
}

void PlotFlame(
    const char *label,
    const std::vector<FlameGraphSample> &samples,
    const std::optional<std::string> &overlay_text,
    float scale_min,
    float scale_max,
    ImVec2 graph_size) {
  auto *window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return;

  const auto &g = *GImGui;
  const auto &style = g.Style;

  ImU8 maxDepth = 0;
  if (const auto max = std::ranges::max_element(samples, [](const auto &a, const auto &b) {
      return a.getMaxDepth() < b.getMaxDepth();
    });
      max != samples.end()) {
    maxDepth = max->getMaxDepth();
  }

  const auto blockHeight =
      ImGui::GetTextLineHeight() + (style.FramePadding.y * 2);
  const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
  if (graph_size.x == 0.0f)
    graph_size.x = ImGui::CalcItemWidth();
  if (graph_size.y == 0.0f)
    graph_size.y = label_size.y + (style.FramePadding.y * 3) + blockHeight * (maxDepth + 1);

  const ImRect frame_bb(window->DC.CursorPos,
                        window->DC.CursorPos + graph_size);
  const ImRect inner_bb(frame_bb.Min + style.FramePadding,
                        frame_bb.Max - style.FramePadding);
  const ImRect total_bb(frame_bb.Min,
                        frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));
  ImGui::ItemSize(total_bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(total_bb, 0, &frame_bb))
    return;

  // Determine scale from values if not specified
  if (scale_min == FLT_MAX || scale_max == FLT_MAX) {
    float v_min = FLT_MAX;
    float v_max = -FLT_MAX;

    if (const auto min = std::ranges::min_element(samples, [](const auto &a, const auto &b) {
        return a.getTime().start < b.getTime().start;
      });
        min != samples.end()) {
      v_min = min->getTime().start.count();
    }
    if (const auto max = std::ranges::max_element(samples, [](const auto &a, const auto &b) {
        return a.getTime().end < b.getTime().end;
      });
        max != samples.end()) {
      v_max = max->getTime().end.count();
    }
    if (scale_min == FLT_MAX)
      scale_min = v_min;
    if (scale_max == FLT_MAX)
      scale_max = v_max;
  }

  ImGui::RenderFrame(frame_bb.Min, frame_bb.Max,
                     ImGui::GetColorU32(ImGuiCol_FrameBg), true,
                     style.FrameRounding);

  bool any_hovered = false;
  if (!samples.empty()) {
    const ImU32 col_base =
        ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x77FFFFFF;
    const ImU32 col_hovered =
        ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x77FFFFFF;
    const ImU32 col_outline_base =
        ImGui::GetColorU32(ImGuiCol_PlotHistogram) & 0x7FFFFFFF;
    const ImU32 col_outline_hovered =
        ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered) & 0x7FFFFFFF;

    std::function<void(const FlameGraphSample &)> renderSample;

    renderSample = ([&](const auto &sample) {
      const auto stageStart = sample.getTime().start;
      const auto stageEnd = sample.getTime().end;
      const ImU8 depth = sample.getLevel();
      const auto caption = sample.getCaption();

      const auto duration = scale_max - scale_min;
      if (duration == 0) {
        return;
      }

      const auto start = stageStart.count() - scale_min;
      const auto end = stageEnd.count() - scale_min;

      const auto startX = start / (double) duration;
      const auto endX = end / (double) duration;

      const auto width = inner_bb.Max.x - inner_bb.Min.x;
      const auto height =
          blockHeight * (maxDepth - depth + 1) - style.FramePadding.y;

      const auto pos0 = inner_bb.Min + ImVec2(startX * width, height);
      const auto pos1 = inner_bb.Min + ImVec2(endX * width, height + blockHeight);

      auto v_hovered = false;
      if (ImGui::IsMouseHoveringRect(pos0, pos1)) {
        ImGui::SetTooltip("%s: %8.4g", caption.c_str(), static_cast<double>(stageEnd.count() - stageStart.count()));
        v_hovered = true;
        any_hovered = v_hovered;
      }

      window->DrawList->AddRectFilled(pos0, pos1,
                                      v_hovered ? col_hovered : col_base);
      window->DrawList->AddRect(
          pos0, pos1, v_hovered ? col_outline_hovered : col_outline_base);
      auto textSize = ImGui::CalcTextSize(caption.c_str());
      auto boxSize = (pos1 - pos0);
      auto textOffset = ImVec2(0.0f, 0.0f);
      if (textSize.x < boxSize.x) {
        textOffset = ImVec2(0.5f, 0.5f) * (boxSize - textSize);
        ImGui::RenderText(pos0 + textOffset, caption.c_str());
      }
      std::ranges::for_each(sample.getSubSamples(), renderSample);
    });

    std::ranges::for_each(samples,
                          renderSample);

    // Text overlay
    if (overlay_text.has_value()) {
      ImGui::RenderTextClipped(
          ImVec2(frame_bb.Min.x, frame_bb.Min.y + style.FramePadding.y),
          frame_bb.Max, overlay_text->c_str(), nullptr, nullptr, ImVec2(0.5f, 0.0f));
    }

    if (label_size.x > 0.0f) {
      ImGui::RenderText(
          ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y),
          label);
    }
  }

  if (!any_hovered && ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Total: %8.4g", scale_max - scale_min);
  }
}

}// namespace ImGuiWidgetFlameGraph