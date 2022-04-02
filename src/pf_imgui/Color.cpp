//
// Created by xflajs00 on 02.04.2022.
//

#include "Color.h"

namespace pf::ui::ig {

const Color Color::Red = Color{ImVec4{1, 0, 0, 1}};
const Color Color::Green = Color{ImVec4{0, 1, 0, 1}};
const Color Color::Blue = Color{ImVec4{0, 0, 1, 1}};
const Color Color::Black = Color{ImVec4{0, 0, 0, 1}};
const Color Color::White = Color{ImVec4{1, 1, 1, 1}};

Color::Color() : color(IM_COL32(255, 255, 255, 255)) {}

Color::Color(ImColor value) : color(ImGui::ColorConvertFloat4ToU32(value)) {}

Color::Color(const ImVec4 &value) : color(ImGui::ColorConvertFloat4ToU32(value)) {}

float Color::red() const { return static_cast<ImVec4>(ImColor{color}).x; }

float Color::green() const { return static_cast<ImVec4>(ImColor{color}).y; }

float Color::blue() const { return static_cast<ImVec4>(ImColor{color}).z; }

float Color::alpha() const { return static_cast<ImVec4>(ImColor{color}).w; }

Color::operator ImU32() const { return color; }

}  // namespace pf::ui::ig