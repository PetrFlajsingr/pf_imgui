//
// Created by petr.flajsingr on 2/14/2022.
//

#include "LinkText.h"

namespace pf::ui::ig {

LinkText::LinkText(LinkText::Config &&config)
    : LinkText(config.name, config.label, config.link, config.clickHandler.value) {}

void LinkText::renderImpl() {
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  {
    if (*hovered) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_SliderGrab]);
    } else {
      ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive]);
    }
    pf::RAII textColPop{[] { ImGui::PopStyleColor(); }};
    ImGui::Text("%s", label->get().c_str());
  }
  auto min = ImGui::GetItemRectMin();
  const auto max = ImGui::GetItemRectMax();
  min.y = max.y;
  if (ImGui::IsItemHovered()) {
    if (ImGui::IsMouseClicked(0)) {
      linkClickHandler(link);
      clickEvent.notify();
    }
    const auto underlineColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_SliderGrab]);
    ImGui::GetWindowDrawList()->AddLine(min, max, underlineColor, 1.0f);
  } else {
    const auto underlineColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive]);
    ImGui::GetWindowDrawList()->AddLine(min, max, underlineColor, 1.0f);
  }
}

}  // namespace pf::ui::ig
