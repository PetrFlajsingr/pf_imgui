/**
 * @file Color.h
 * @brief Color.
 * @author Petr Flajšingr
 * @date 2.4.22
 */

#ifndef PF_IMGUI_COLOR_H
#define PF_IMGUI_COLOR_H

#include <concepts>
#include <imgui.h>
#include <pf_common/concepts/Integral.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

/**
 * @brief Color for pf_imgui, optimized compared to using ImVec4.
 * TODO: blend functions etc
 */
class PF_IMGUI_EXPORT Color {
 public:
  constexpr Color() : color(IM_COL32(255, 255, 255, 255)) {}
  explicit Color(ImColor value) : color(value) {}
  constexpr explicit Color(ImU32 value) : color(value) {}
  explicit Color(const ImVec4 &value) : color(ImGui::ColorConvertFloat4ToU32(value)) {}
  // color constants
  // TODO: add more
  const static Color Red;
  const static Color Green;
  const static Color Blue;
  const static Color Black;
  const static Color White;

  [[nodiscard]] constexpr bool operator==(const Color &rhs) const { return color == rhs.color; }
  [[nodiscard]] constexpr bool operator!=(const Color &rhs) const { return !(rhs == *this); }

  [[nodiscard]] constexpr static Color RGB(Integral auto red, Integral auto green, Integral auto blue,
                                           Integral auto alpha) {
    return Color{IM_COL32(red, green, blue, alpha)};
  }
  [[nodiscard]] constexpr static Color RGB(Integral auto red, Integral auto green, Integral auto blue) {
    return Color{IM_COL32(red, green, blue, 255)};
  }
  [[nodiscard]] constexpr static Color RGB(std::same_as<float> auto red, std::same_as<float> auto green,
                                           std::same_as<float> auto blue, std::same_as<float> auto alpha) {
    return Color{ImColor(red, green, blue, alpha)};
  }
  [[nodiscard]] constexpr static Color RGB(std::same_as<float> auto red, std::same_as<float> auto green,
                                           std::same_as<float> auto blue) {
    return Color{ImColor(red, green, blue, 1.f)};
  }

  [[nodiscard]] constexpr static Color HSV(Integral auto hue, Integral auto saturation, Integral auto value,
                                           Integral auto alpha = 255u) {
    float r;
    float g;
    float b;
    ImGui::ColorConvertHSVtoRGB(static_cast<float>(hue) / 255.f, static_cast<float>(saturation) / 255.f,
                                static_cast<float>(value) / 255.f, r, g, b);
    return RGB(r, g, b, alpha);
  }
  [[nodiscard]] constexpr static Color HSV(std::same_as<float> auto hue, std::same_as<float> auto saturation,
                                           std::same_as<float> auto value, std::same_as<float> auto alpha) {
    float r;
    float g;
    float b;
    ImGui::ColorConvertHSVtoRGB(hue, saturation, value, r, g, b);
    return RGB(r, g, b, alpha);
  }
  [[nodiscard]] constexpr static Color HSV(std::same_as<float> auto hue, std::same_as<float> auto saturation,
                                           std::same_as<float> auto value) {
    return HSV(hue, saturation, value, 1.f);
  }

  [[nodiscard]] constexpr float red() const { return static_cast<ImVec4>(ImColor{color}).x; }
  [[nodiscard]] constexpr float green() const { return static_cast<ImVec4>(ImColor{color}).y; }
  [[nodiscard]] constexpr float blue() const { return static_cast<ImVec4>(ImColor{color}).z; }
  [[nodiscard]] constexpr float alpha() const { return static_cast<ImVec4>(ImColor{color}).w; }

  constexpr void setRed(Integral auto red) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.x = static_cast<float>(red) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  constexpr void setGreen(Integral auto green) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.y = static_cast<float>(green) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  constexpr void setBlue(Integral auto blue) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.z = static_cast<float>(blue) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  constexpr void setAlpha(Integral auto alpha) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.w = static_cast<float>(alpha) / 1.f;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }

  constexpr void setRed(std::same_as<float> auto red) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.x = red;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  constexpr void setGreen(std::same_as<float> auto green) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.y = green;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  constexpr void setBlue(std::same_as<float> auto blue) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.z = blue;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }
  constexpr void setAlpha(std::same_as<float> auto alpha) {
    auto col = static_cast<ImVec4>(ImColor{color});
    col.w = alpha;
    color = ImGui::ColorConvertFloat4ToU32(col);
  }

  constexpr explicit operator ImU32() const { return color; }

  constexpr explicit operator ImVec4() const { return static_cast<ImVec4>(ImColor{color}); }

 private:
  ImU32 color;
};

constexpr inline Color Color::Red = Color::RGB(255, 0, 0, 255);
constexpr inline Color Color::Green = Color::RGB(0, 255, 0, 255);
constexpr inline Color Color::Blue = Color::RGB(0, 0, 255, 255);
constexpr inline Color Color::Black = Color::RGB(0, 0, 0, 255);
constexpr inline Color Color::White = Color::RGB(255, 255, 255, 255);

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COLOR_H
