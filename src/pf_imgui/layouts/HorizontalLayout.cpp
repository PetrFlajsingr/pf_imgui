//
// Created by xflajs00 on 10.04.2022.
//

#include "HorizontalLayout.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

HorizontalLayout::HorizontalLayout(HorizontalLayout::Config &&config)
    : LinearLayout(std::string{config.name}, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No),
      alignment(config.align) {}

HorizontalLayout::HorizontalLayout(const std::string &name, Size size, HorizontalAlign align, ShowBorder showBorder)
    : LinearLayout(name, size, showBorder), alignment(align) {}

void HorizontalLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    ImGui::BeginHorizontal(getName().c_str(), ImVec2{0, 0}, alignmentAsFloat());
    auto endLayout = RAII{ImGui::EndHorizontal};
    {
      auto elements = getChildren();
      std::ranges::for_each_n(elements.begin(), static_cast<int>(elements.size() - 1), [this](auto &child) {
        child.render();
        ImGui::Spring(0.f, ImGui::GetStyle().ItemSpacing.x);
      });
      if (!elements.empty()) { elements.back().render(); }
    }
  }
}
float HorizontalLayout::alignmentAsFloat() const {
  switch (alignment) {
    case HorizontalAlign::Up: return 0.f;
    case HorizontalAlign::Middle: return -1.f;
    case HorizontalAlign::Down: return 1.f;
  }
  assert(false && "This can not happen");
  return 0;
}

}  // namespace pf::ui::ig