//
// Created by xflajs00 on 10.04.2022.
//

#include "HorizontalLayout.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

HorizontalLayout::HorizontalLayout(HorizontalLayout::Config &&config)
    : HorizontalLayout(config.name, config.size, config.align, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {}

HorizontalLayout::HorizontalLayout(std::string_view elementName, Size initialSize, HorizontalAlign align,
                                   ShowBorder showBorder)
    : LinearLayout(elementName, initialSize, showBorder), alignment(align) {}

void HorizontalLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
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