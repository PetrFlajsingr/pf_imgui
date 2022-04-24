//
// Created by xflajs00 on 10.04.2022.
//

#include "LinearLayout.h"
#include <pf_common/exceptions/Exceptions.h>
#include <range/v3/range/conversion.hpp>

namespace pf::ui::ig {

LinearLayout::LinearLayout(const std::string &name, Size size, ShowBorder showBorder)
    : ResizableLayout(name, size, showBorder) {}

void LinearLayout::pushChild(std::unique_ptr<Element> child) { children.emplace_back(std::move(child)); }

void LinearLayout::insertChild(std::unique_ptr<Element> child, std::size_t index) {
  children.insert(children.begin() + static_cast<long long>(index), std::move(child));
}
void LinearLayout::removeChild(const std::string &childName) {
  if (auto iter =
          std::ranges::find_if(children, [childName](const auto &child) { return child->getName() == childName; });
      iter != children.end()) {
    children.erase(iter);
  }
}

std::vector<Renderable *> LinearLayout::getRenderables() {
  return children | ranges::views::transform([](auto &child) -> Renderable * { return child.get(); })
      | ranges::to_vector;
}

}  // namespace pf::ui::ig