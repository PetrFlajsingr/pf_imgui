//
// Created by petr on 10/31/20.
//

#include "Image.h"

#include <utility>

namespace pf::ui::ig {

Image::Image(const std::string &elementName, ImTextureID imTextureId, const ImVec2 &size, IsButton isBtn,
             Image::UvMappingProvider uvTextureMappingProvider)
    : Element(elementName), ResizableElement(elementName, size), isButton_(isBtn == IsButton::Yes),
      textureId(imTextureId), uvMappingProvider(std::move(uvTextureMappingProvider)) {}

void Image::renderImpl() {
  const auto [uvStart, uvEnd] = uvMappingProvider();
  if (isButton_) {
    if (ImGui::ImageButton(textureId, getSize(), uvStart, uvEnd)) { notifyOnClick(); }
  } else {
    ImGui::Image(textureId, getSize(), uvStart, uvEnd);
  }
}

void Image::setTextureId(ImTextureID imTetureId) { textureId = imTetureId; }

bool Image::isButton() const { return isButton_; }

}// namespace pf::ui::ig