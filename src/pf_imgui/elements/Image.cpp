//
// Created by petr on 10/31/20.
//

#include "Image.h"
#include <utility>

namespace pf::ui::ig {

Image::Image(const std::string &elementName, ImTextureID imTextureId, const Size &size, IsButton isBtn,
             Image::UvMappingProvider uvTextureMappingProvider)
    : ItemElement(elementName), Resizable(size), isButton_(isBtn == IsButton::Yes), textureId(imTextureId),
      uvMappingProvider(std::move(uvTextureMappingProvider)) {}

void Image::renderImpl() {
  const auto [uvStart, uvEnd] = uvMappingProvider();
  if (isButton_) {
    if (ImGui::ImageButton(textureId, getSize().asImVec(), uvStart, uvEnd)) { notifyOnClick(); }
  } else {
    ImGui::Image(textureId, getSize().asImVec(), uvStart, uvEnd);
  }
}

void Image::setTextureId(ImTextureID imTextureId) { textureId = imTextureId; }

bool Image::isButton() const { return isButton_; }

}// namespace pf::ui::ig