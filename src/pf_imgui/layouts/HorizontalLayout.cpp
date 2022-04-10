//
// Created by xflajs00 on 10.04.2022.
//

#include "HorizontalLayout.h"
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

HorizontalLayout::HorizontalLayout(HorizontalLayout::Config &&config)
    : ResizableLayout(std::string{config.name}, config.size, config.showBorder) {}

HorizontalLayout::HorizontalLayout(const std::string &name, Size size, float elementSpacing, ShowBorder showBorder)
    : ResizableLayout(name, size, showBorder), spacing(elementSpacing) {}

float HorizontalLayout::getSpacing() const { return spacing; }

void HorizontalLayout::setSpacing(float newSpacing) { spacing = newSpacing; }

void HorizontalLayout::pushChild(std::unique_ptr<Element> child) { children.emplace_back(std::move(child)); }

void HorizontalLayout::insertChild(std::unique_ptr<Element> child, std::size_t index) {
#ifndef _MSC_VER  // TODO: MSVC internal error
  if (index > children.size()) { throw InvalidArgumentException("Index out of bounds: {}", index); }
#endif
  children.insert(children.begin() + static_cast<long long>(index), std::move(child));
}
void HorizontalLayout::removeChild(const std::string &childName) {
  if (auto iter =
          std::ranges::find_if(children, [childName](const auto &child) { return child->getName() == childName; });
      iter != children.end()) {
    children.erase(iter);
  }
}

std::vector<Renderable *> HorizontalLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); })
      | ranges::to_vector;
}

void HorizontalLayout::renderImpl() {
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    ImGui::BeginHorizontal(getName().c_str());
    auto endLayout = RAII{ImGui::EndHorizontal};
    {
      std::ranges::for_each_n(children.begin(), static_cast<int>(children.size() - 1), [this](auto &child) {
        child->render();
        ImGui::Spring(0.f, spacing);
      });
      if (!children.empty()) { children.back()->render(); }
    }
  }
}

}  // namespace pf::ui::ig