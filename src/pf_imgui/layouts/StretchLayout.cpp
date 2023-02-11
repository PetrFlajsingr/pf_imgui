//
// Created by petr on 1/25/21.
//

#include "StretchLayout.h"
#include <string>
#include <vector>

namespace pf::ui::ig {

StretchLayout::StretchLayout(StretchLayout::Config &&config)
    : StretchLayout(config.name, config.size, config.stretch, config.showBorder ? ShowBorder::Yes : ShowBorder::No) {}

StretchLayout::StretchLayout(std::string_view elementName, Size initialSize, Stretch stretchType, ShowBorder showBorder)
    : Layout(elementName, initialSize, showBorder), stretch(stretchType) {}

Stretch StretchLayout::getStretch() const { return stretch; }

void StretchLayout::setStretch(Stretch newStretch) {
  stretch = newStretch;
  switch (stretch) {
    case Stretch::Width: *size.modify() = {Width::Auto(), size->height}; break;
    case Stretch::Height: *size.modify() = {size->width, Height::Auto()}; break;
    case Stretch::All: *size.modify() = Size::Auto(); break;
  }
}

std::optional<std::reference_wrapper<Element>> StretchLayout::getChild() {
  if (child == nullptr) { return std::nullopt; }
  return *dynamic_cast<Element *>(child.get());
}

void StretchLayout::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  ScopeExit end{&ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(*size), isDrawBorder(), flags)) {
    if (child == nullptr) { return; }
    auto scrollApplier = applyScroll();
    const auto newSize = ImGui::GetContentRegionMax();
    if (newSize.x != previousSize.x && newSize.y != previousSize.y) {  //-V550
      updateChildSize(newSize);
      previousSize = newSize;
    }
    child->render();
  }
}

std::vector<Renderable *> StretchLayout::getRenderables() { return {dynamic_cast<Renderable *>(child.get())}; }

bool StretchLayout::hasChild() const { return child != nullptr; }

}  // namespace pf::ui::ig
