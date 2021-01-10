//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H
#define PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H

#include "../enums.h"
#include "interface/Clickable.h"
#include "interface/ResizableElement.h"
#include <functional>
#include <pf_imgui/_export.h>
#include <string>
#include <utility>

namespace pf::ui::ig {
/**
 * ImTextureId is basically anything.
 * For example in Vulkan implementation it could be a descriptor set.
 */
class PF_IMGUI_EXPORT Image : public ResizableElement, public Clickable {
 public:
  using UvMappingProvider = std::function<std::pair<ImVec2, ImVec2>()>;

  Image(
      const std::string &elementName, ImTextureID imTextureId, const ImVec2 &size, IsButton isBtn = IsButton::No,
      UvMappingProvider uvTextureMappingProvider = [] {
        return std::pair(ImVec2{0, 0}, ImVec2{1, 1});
      });

  [[nodiscard]] bool isButton() const;

  void setTextureId(ImTextureID textureId);

 protected:
  void renderImpl() override;

 private:
  bool isButton_;
  ImTextureID textureId;
  UvMappingProvider uvMappingProvider;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_IMAGE_H
