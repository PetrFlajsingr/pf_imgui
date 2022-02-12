//
// Created by petr on 1/25/21.
//

#include "StretchLayout.h"
#include <pf_common/exceptions/Exceptions.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

StretchLayout::StretchLayout(StretchLayout::Config &&config)
    : ResizableLayout(std::string{config.name}, config.size, config.allowCollapse, config.showBorder,
                      config.persistent),
      stretch(config.stretch) {}

StretchLayout::StretchLayout(const std::string &elementName, const Size &size, Stretch stretch,
                             AllowCollapse allowCollapse, ShowBorder showBorder, Persistent persistent)
    : ResizableLayout(elementName, size, allowCollapse, showBorder, persistent), stretch(stretch) {}

Stretch StretchLayout::getStretch() const { return stretch; }
void StretchLayout::setStretch(Stretch newStretch) {
  stretch = newStretch;
  switch (stretch) {
    case Stretch::Width: setSize({Width::Auto(), getSize().height}); break;
    case Stretch::Height: setSize({getSize().width, Height::Auto()}); break;
    case Stretch::All: setSize(Size::Auto()); break;
  }
}

Element &StretchLayout::getChild() {
#ifndef _MSC_VER  // TODO: MSVC internal error
  if (child == nullptr) { throw Exception("Child not present"); }
#endif
  return *dynamic_cast<Element *>(child.get());
}

void StretchLayout::renderImpl() {
  [[maybe_unused]] auto colorStyle = setColorStack();
  [[maybe_unused]] auto style = setStyleStack();
  const auto flags =
      isScrollable() ? ImGuiWindowFlags_{} : ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), getSizeIfCollapsed(), isDrawBorder(), flags)) {
    if (renderCollapseButton()) {
      const auto newSize = ImGui::GetContentRegionMax();
      if (newSize.x != previousSize.x && newSize.y != previousSize.y) {
        child->setSize(newSize);
        previousSize = newSize;
      }
      renderableChild->render();
    }
  }
}

std::vector<Renderable *> StretchLayout::getRenderables() { return {dynamic_cast<Renderable *>(child.get())}; }

bool StretchLayout::hasChild() const { return child != nullptr; }

}  // namespace pf::ui::ig
