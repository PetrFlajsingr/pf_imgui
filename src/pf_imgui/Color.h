/**
 * @file Color.h
 * @brief Color.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_IMGUI_COLOR_H
#define PF_IMGUI_COLOR_H

#include <concepts>
#include <cstdint>
#include <imgui.h>
#include <pf_imgui/_export.h>
#include <pf_common/concepts/Integral.h>

namespace pf::ui::ig {

/**
 * @brief Color for pf_imgui, optimized compared to using ImVec4.
 * TODO: blend functions etc
 */
class PF_IMGUI_EXPORT Color {
 public:
  Color();
  explicit Color(ImColor value);
  explicit Color(ImU32 value);
  explicit Color(const ImVec4 &value);
  // color constants
  // TODO: add more
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Black;
  static const Color White;

  [[nodiscard]] bool operator==(const Color &rhs) const;
  [[nodiscard]] bool operator!=(const Color &rhs) const;

  [[nodiscard]] static Color RGB(Integral auto red, Integral auto green,
                                 Integral auto blue, Integral auto alpha) {
    return Color{IM_COL32(red, green, blue, alpha)};
  }
  [[nodiscard]] static Color RGB(Integral auto red, Integral auto green,
                                 Integral auto blue) {
    return Color{IM_COL32(red, green, blue, 255)};
  }
  [[nodiscard]] static Color RGB(std::same_as<float> auto red, std::same_as<float> auto green,
                                 std::same_as<float> auto blue, std::same_as<float> auto alpha) {
    return Color{ImColor(red, green, blue, alpha)};
  }
  [[nodiscard]] static Color RGB(std::same_as<float> auto red, std::same_as<float> auto green,
                                 std::same_as<float> auto blue) {
    return Color{ImColor(red, green, blue, 1.f)};
  }

  [[nodiscard]] static Color HSV(Integral auto hue, Integral auto saturation,
                                 Integral auto value, Integral auto alpha = 255u) {
    float r;
    float g;
    float b;
    ImGui::ColorConvertHSVtoRGB(static_cast<float>(hue) / 255.f, static_cast<float>(saturation) / 255.f,
                                static_cast<float>(value) / 255.f, r, g, b);
    return RGB(r, g, b, alpha);
  }
  [[nodiscard]] static Color HSV(std::same_as<float> auto hue, std::same_as<float> auto saturation,
                                 std::same_as<float> auto value, std::same_as<float> auto alpha) {
    float r;
    float g;
    float b;
    ImGui::ColorConvertHSVtoRGB(hue, saturation, value, r, g, b);
    return RGB(r, g, b, alpha);
  }
  [[nodiscard]] static Color HSV(std::same_as<float> auto hue, std::same_as<float> auto saturation,
                                 std::same_as<float> auto value) {
    return HSV(hue, saturation, value, 1.f);
  }

  [[nodiscard]] float red() const;
  [[nodiscard]] float green() const;
  [[nodiscard]] float blue() const;
  [[nodiscard]] float alpha() const;

  void setRed(Integral auto red) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.x = static_cast<float>(red) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  void setGreen(Integral auto green) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.y = static_cast<float>(green) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  void setBlue(Integral auto blue) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.z = static_cast<float>(blue) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  void setAlpha(Integral auto alpha) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.w = static_cast<float>(alpha) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }

  void setRed(std::same_as<float> auto red) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.x = red;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  void setGreen(std::same_as<float> auto green) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.y = green;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  void setBlue(std::same_as<float> auto blue) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.z = blue;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  void setAlpha(std::same_as<float> auto alpha) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.w = alpha;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }

  operator ImU32() const;

  explicit operator ImVec4() const;

 private:
  ImU32 color;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COLOR_H
