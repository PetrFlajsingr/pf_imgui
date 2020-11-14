//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H
#define PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H

#include "../enums.h"
#include "interface/ResizableElement.h"
#include <functional>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Image : public ResizableElement {
 public:
  using ImTextureIDProvider = std::function<ImTextureID()>;
  using UvMappingProvider = std::function<std::pair<ImVec2, ImVec2>()>;

  Image(
      const std::string &elementName, ImTextureIDProvider imTextureIdProvider, const ImVec2 &size,
      IsButton isBtn = IsButton::No, UvMappingProvider uvTextureMappingProvider = [] {
        return std::pair(ImVec2{0, 0}, ImVec2{1, 1});
      });

  [[nodiscard]] bool isButton() const;

  void setOnClick(std::invocable auto fnc) { onClick = fnc; }

 protected:
  void renderImpl() override;

 private:
  bool isButton_;
  ImTextureIDProvider textureIdProvider;
  UvMappingProvider uvMappingProvider;

  std::function<void()> onClick = [] {};
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H
