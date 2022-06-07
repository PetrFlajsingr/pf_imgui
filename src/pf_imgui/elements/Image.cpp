//
// Created by petr on 10/31/20.
//

#include "Image.h"
#include <utility>

namespace pf::ui::ig {

Image::Image(Image::Config &&config)
    : ItemElement(std::string{config.name.value}), Resizable(config.size), textureId(config.textureId) {}

Image::Image(const std::string &elementName, ImTextureID imTextureId, Size size)
    : ItemElement(elementName), Resizable(size), textureId(imTextureId) {}

void Image::setUVs(ImVec2 leftTop, ImVec2 rightBottom) {
  uvLeftTop = leftTop;
  uvRightBottom = rightBottom;
}

void Image::renderImpl() { ImGui::Image(textureId, static_cast<ImVec2>(getSize()), uvLeftTop, uvRightBottom); }

void Image::setTextureId(ImTextureID imTextureId) { textureId = imTextureId; }

}  // namespace pf::ui::ig
