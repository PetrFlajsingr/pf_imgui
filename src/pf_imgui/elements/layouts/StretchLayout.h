//
// Created by petr on 1/25/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H
#define PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H

#include "ResizableLayout.h"
#include <memory>
#include <pf_imgui/enums.h>

namespace pf::ui::ig {
class StretchLayout : public ResizableLayout {
 public:
  StretchLayout(const std::string &elementName, Stretch stretch, const ImVec2 &size, bool showBorder = false);

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::derived_from<T, Resizable> &&
      std::constructible_from<T, std::string, Args...>
          T &createChild(std::string name, Args &&...args) {
    child = std::make_unique<T>(name, std::forward<Args>(args)...);
    return *child;
  }

  std::vector<Renderable *> getRenderables() override;

 public:
  [[nodiscard]] Element &getChild();

  [[nodiscard]] Stretch getStretch() const;
  void setStretch(Stretch newStretch);

 protected:
  void renderImpl() override;

 private:
  Stretch stretch;
  std::unique_ptr<Resizable> child;


};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H
