//
// Created by petr on 10/31/20.
//

#include "Image.h"

#include <utility>

namespace pf::ui::ig {

Image::Image(const std::string &elementName, Image::ImTextureIDProvider imTextureIdProvider, const ImVec2 &size,
             IsButton isBtn, Image::UvMappingProvider uvTextureMappingProvider)
    : Element(elementName), ResizableElement(elementName, size), isButton_(isBtn == IsButton::Yes),
      textureIdProvider(std::move(imTextureIdProvider)), uvMappingProvider(std::move(uvTextureMappingProvider)) {}

void Image::renderImpl() {
  const auto [uvStart, uvEnd] = uvMappingProvider();
  const auto textureId = textureIdProvider();
  if (isButton_) {
    if (ImGui::ImageButton(textureId, getSize(), uvStart, uvEnd)) { notifyOnClick(); }
  } else {
    ImGui::Image(textureId, getSize(), uvStart, uvEnd);
  }
}

bool Image::isButton() const { return isButton_; }
}// namespace pf::ui::ig