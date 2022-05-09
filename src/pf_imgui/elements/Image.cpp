//
// Created by petr on 10/31/20.
//

#include "Image.h"
#include <utility>

namespace pf::ui::ig {

Image::Image(Image::Config &&config)
    : ItemElement(std::string{config.name}), Resizable(config.size), isButton_(config.isButton),
      textureId(config.textureId), uvMappingProvider(std::move(config.uvTextureMappingProvider)) {}

Image::Image(const std::string &elementName, ImTextureID imTextureId, Size size, IsButton isBtn,
             Image::UvMappingProvider uvTextureMappingProvider)
    : ItemElement(elementName), Resizable(size), isButton_(isBtn == IsButton::Yes), textureId(imTextureId),
      uvMappingProvider(std::move(uvTextureMappingProvider)) {}

void Image::renderImpl() {
  auto colorStyle = setColorStack();
  const auto [uvStart, uvEnd] = uvMappingProvider();

  if (isButton_) {
    if (ImGui::ImageButton(textureId, static_cast<ImVec2>(getSize()), uvStart, uvEnd)) { notifyOnClick(); }
  } else {
    ImGui::Image(textureId, static_cast<ImVec2>(getSize()), uvStart, uvEnd);
  }
}

void Image::setTextureId(ImTextureID imTextureId) { textureId = imTextureId; }
bool Image::isButton() const { return isButton_; }

}  // namespace pf::ui::ig
