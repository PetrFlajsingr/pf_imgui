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
  StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch,
                AllowCollapse allowCollapse = AllowCollapse::No, ShowBorder showBorder = ShowBorder::No,
                Persistent persistent = Persistent::No);
  StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch, ShowBorder showBorder,
                Persistent persistent = Persistent::No);
  StretchLayout(const std::string &elementName, const ImVec2 &size, Stretch stretch, AllowCollapse allowCollapse,
                Persistent persistent = Persistent::No);

  template<typename T, typename... Args>
  requires std::derived_from<T, Element> &&std::derived_from<T, Resizable> &&
      std::constructible_from<T, std::string, Args...>
          T &createChild(std::string name, Args &&...args) {
    child = std::make_unique<T>(name, std::forward<Args>(args)...);
    renderableChild = dynamic_cast<Renderable *>(child.get());
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
  std::unique_ptr<Resizable> child = nullptr;
  Renderable *renderableChild;
};
}// namespace pf::ui::ig

#endif//PF_IMGUI_SRC_PF_IMGUI_ELEMENTS_LAYOUTS_STRETCHLAYOUT_H
