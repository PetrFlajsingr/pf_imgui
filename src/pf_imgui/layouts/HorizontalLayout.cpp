//
// Created by xflajs00 on 10.04.2022.
//

#include "HorizontalLayout.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

HorizontalLayout::HorizontalLayout(HorizontalLayout::Config &&config)
    : LinearLayout(std::string{config.name}, config.size, config.showBorder), spacing(config.spacing) {}

HorizontalLayout::HorizontalLayout(const std::string &name, Size size, float elementSpacing, ShowBorder showBorder)
    : LinearLayout(name, size, showBorder), spacing(elementSpacing) {}

float HorizontalLayout::getSpacing() const { return spacing; }

void HorizontalLayout::setSpacing(float newSpacing) { spacing = newSpacing; }

void HorizontalLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    ImGui::BeginHorizontal(getName().c_str());
    auto endLayout = RAII{ImGui::EndHorizontal};
    {
      auto elements = getChildren();
      std::ranges::for_each_n(elements.begin(), static_cast<int>(elements.size() - 1), [this](auto &child) {
        child.render();
        ImGui::Spring(0.f, spacing);
      });
      if (!elements.empty()) { elements.back().render(); }
    }
  }
}

}  // namespace pf::ui::ig