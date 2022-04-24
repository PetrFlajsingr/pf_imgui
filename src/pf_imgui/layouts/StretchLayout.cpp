//
// Created by petr on 1/25/21.
//

#include "StretchLayout.h"
#include <pf_common/exceptions/Exceptions.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

StretchLayout::StretchLayout(StretchLayout::Config &&config)
    : ResizableLayout(std::string{config.name}, config.size, config.showBorder ? ShowBorder::Yes : ShowBorder::No),
      stretch(config.stretch) {}

StretchLayout::StretchLayout(const std::string &elementName, const Size &size, Stretch stretch, ShowBorder showBorder)
    : ResizableLayout(elementName, size, showBorder), stretch(stretch) {}

Stretch StretchLayout::getStretch() const { return stretch; }
void StretchLayout::setStretch(Stretch newStretch) {
  stretch = newStretch;
  switch (stretch) {
    case Stretch::Width: setSize({Width::Auto(), getSize().height}); break;
    case Stretch::Height: setSize({getSize().width, Height::Auto()}); break;
    case Stretch::All: setSize(Size::Auto()); break;
  }
}

std::optional<std::reference_wrapper<Element>> StretchLayout::getChild() {
  if (child == nullptr) { std::nullopt; }
  return *dynamic_cast<Element *>(child.get());
}

void StretchLayout::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), static_cast<ImVec2>(getSize()), isDrawBorder(), flags)) {
    auto scrollApplier = applyScroll();
    const auto newSize = ImGui::GetContentRegionMax();
    if (newSize.x != previousSize.x && newSize.y != previousSize.y) {
      child->setSize(newSize);
      previousSize = newSize;
    }
    if (renderableChild != nullptr) { renderableChild->render(); }
  }
}

std::vector<Renderable *> StretchLayout::getRenderables() { return {dynamic_cast<Renderable *>(child.get())}; }

bool StretchLayout::hasChild() const { return child != nullptr; }

}  // namespace pf::ui::ig
