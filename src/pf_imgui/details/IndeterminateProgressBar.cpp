//
// Created by xflajs00 on 21.05.2022.
//

#include "IndeterminateProgressBar.h"
#include <imgui_internal.h>

void ImGui::IndeterminateProgressBar(float fraction, const ImVec2 &size_arg, const char *overlay) {
  ImGuiWindow *window = GetCurrentWindow();
  if (window->SkipItems) return;

  ImGuiContext &g = *GImGui;
  const ImGuiStyle &style = g.Style;

  ImVec2 pos = window->DC.CursorPos;
  ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
  ImRect bb(pos, pos + size);
  ItemSize(size, style.FramePadding.y);
  if (!ItemAdd(bb, 0)) return;

  float bar_begin = 0.0f;
  float bar_end = ImSaturate(fraction);

  const bool indeterminate = fraction < 0.0f;
  if (indeterminate) {
    const float bar_fraction = 0.2f;
    bar_begin = ImFmod(-fraction, 1.0f) * (1.0f + bar_fraction) - bar_fraction;
    bar_end = bar_begin + bar_fraction;
  }

  // Render
  RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
  bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
  RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(ImGuiCol_PlotHistogram), bar_begin, bar_end, style.FrameRounding);

  // Don't display text for indeterminate bars by default
  if (!indeterminate || overlay) {
    // Default displaying the fraction as percentage string, but user can override it
    char overlay_buf[32];
    if (!overlay) {
      ImFormatString(overlay_buf, IM_ARRAYSIZE(overlay_buf), "%.0f%%", fraction * 100 + 0.01f);
      overlay = overlay_buf;
    }

    const ImVec2 overlay_size = CalcTextSize(overlay, NULL);
    if (overlay_size.x > 0.0f) {
      const ImVec2 fill_br =
          ImVec2(indeterminate ? (bb.Min.x + bb.Max.x - overlay_size.x) * 0.5f : ImLerp(bb.Min.x, bb.Max.x, bar_end), bb.Max.y);
      RenderTextClipped(
          ImVec2(ImClamp(fill_br.x + style.ItemSpacing.x, bb.Min.x, bb.Max.x - overlay_size.x - style.ItemInnerSpacing.x), bb.Min.y),
          bb.Max, overlay, NULL, &overlay_size, ImVec2(0.0f, 0.5f), &bb);
    }
  }
}
