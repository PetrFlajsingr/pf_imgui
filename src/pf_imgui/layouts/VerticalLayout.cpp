//
// Created by xflajs00 on 10.04.2022.
//

#include "VerticalLayout.h"

namespace pf::ui::ig {

VerticalLayout::VerticalLayout(VerticalLayout::Config &&config)
    : LinearLayout(std::string{config.name.value}, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No),
      alignment(config.align) {}

VerticalLayout::VerticalLayout(const std::string &name, Size size, VerticalAlign align, ShowBorder showBorder)
    : LinearLayout(name, size, showBorder), alignment(align) {}

void VerticalLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    ImGui::BeginVertical(getName().c_str(), ImVec2{0, 0}, alignmentAsFloat());
    auto endLayout = RAII{ImGui::EndVertical};
    {
      auto elements = getChildren();
      std::ranges::for_each_n(elements.begin(), static_cast<int>(elements.size() - 1), [this](auto &child) {
        child.render();
        ImGui::Spring(0.f, ImGui::GetStyle().ItemSpacing.y);
      });
      if (!elements.empty()) { elements.back().render(); }
    }
  }
}

float VerticalLayout::alignmentAsFloat() const {
  switch (alignment) {
    case VerticalAlign::Left: return 0.f;
    case VerticalAlign::Middle: return -1.f;
    case VerticalAlign::Right: return 1.f;
  }
  assert(false && "This can not happen");
  return 0;
}

}  // namespace pf::ui::ig