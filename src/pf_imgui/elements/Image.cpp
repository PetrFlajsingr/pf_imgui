//
// Created by petr on 10/31/20.
//

#include "Image.h"
#include <imgui_internal.h>
#include <utility>

#include <iostream>
namespace pf::ui::ig {

Image::Image(const std::string &elementName, ImTextureID imTextureId, const Size &size, IsButton isBtn,
             bool detectMousePositionEnabled, Image::UvMappingProvider uvTextureMappingProvider)
    : ItemElement(elementName), Resizable(size), isButton_(isBtn == IsButton::Yes), textureId(imTextureId),
      uvMappingProvider(std::move(uvTextureMappingProvider)) {}

void Image::renderImpl() {
  auto colorStyle = setColorStack();
  const auto [uvStart, uvEnd] = uvMappingProvider();

  if (isButton_) {
    if (ImGui::ImageButton(textureId, getSize().asImVec(), uvStart, uvEnd)) { notifyOnClick(); }
  } else {
    ImGui::Image(textureId, getSize().asImVec(), uvStart, uvEnd);
  }
  //if (pixelInspectionTooltipEnabled) {
  //  auto &io = ImGui::GetIO();
  //  auto rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  //  ImVec2 mouseUVCoord = (io.MousePos - rc.Min) / rc.GetSize();
  //  mouseUVCoord.y = 1.f - mouseUVCoord.y;
  //
  //  if (io.KeyShift && io.MouseDown[0] && mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f) {
  //    int width = getSize().width;
  //    int height = getSize().height;
  //
  //    imageInspect(width, height, pickerImage.GetBits(), mouseUVCoord, displayedTextureSize);
  //  }
  //}
}

void Image::setTextureId(ImTextureID imTextureId) { textureId = imTextureId; }

bool Image::isButton() const { return isButton_; }

//bool Image::isPixelInspectionTooltipEnabled() const {
//  return pixelInspectionTooltipEnabled;
//}
//
//void Image::setPixelInspectionTooltipEnabled(bool enabled) {
//  pixelInspectionTooltipEnabled = enabled;
//}

}// namespace pf::ui::ig