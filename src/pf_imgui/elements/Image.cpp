//
// Created by petr on 10/31/20.
//

#include "Image.h"
#include <imgInspect.h>
#include <utility>

namespace pf::ui::ig {

Image::Image(Image::Config &&config)
    : ItemElement(std::string{config.name.value}), Resizable(config.size), textureId(config.textureId) {}

Image::Image(const std::string &elementName, ImTextureID imTextureId, Size size)
    : ItemElement(elementName), Resizable(size), textureId(imTextureId) {}

void Image::renderImpl() { ImGui::Image(textureId, static_cast<ImVec2>(getSize()), uvLeftTop, uvRightBottom); }

void Image::setTextureId(ImTextureID imTextureId) { textureId = imTextureId; }

ImTextureID Image::getTextureId() const { return textureId; }

void Image::setUVs(ImVec2 leftTop, ImVec2 rightBottom) {
  uvLeftTop = leftTop;
  uvRightBottom = rightBottom;
}

InspectableImage::InspectableImage(InspectableImage::Config &&config)
    : InspectableImage(std::string{config.name.value}, config.size, config.rgbaData, config.imageWidth,
                       config.textureId, config.trigger) {}

InspectableImage::InspectableImage(const std::string &elementName, Size s, std::span<const std::byte> rgbaData,
                                   std::size_t imgWidth, ImTextureID texId, Trigger trigger)
    : ItemElement(elementName), Resizable(s), imageRGBAData(rgbaData), imageWidth(imgWidth), textureId(texId),
      trig(trigger) {}

void InspectableImage::renderImpl() {
  const auto cursorPos = ImGui::GetCursorPos();
  ImGui::Image(textureId, static_cast<ImVec2>(getSize()), uvLeftTop, uvRightBottom);
  auto &io = ImGui::GetIO();
  const auto rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  ImGui::SetCursorPos(cursorPos);
  ImGui::InvisibleButton("ignore_btn", static_cast<ImVec2>(getSize()));
  auto mouseUVCoord = (io.MousePos - rc.Min) / rc.GetSize();

  bool inspectActive = ImGui::IsItemHovered();
  switch (trig) {
    case Trigger::Hover: break;
    case Trigger::LeftMouseDown: inspectActive = inspectActive && io.MouseDown[0]; break;
    case Trigger::RightMouseDown: inspectActive = inspectActive && io.MouseDown[2]; break;
  }

  if (inspectActive && mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f) {
    const auto dataPtr = imageRGBAData.data();
    ImageInspect::inspect(static_cast<int>(imageWidth), static_cast<int>(imageRGBAData.size() / 4 / imageWidth - 1),
                          reinterpret_cast<const unsigned char *>(dataPtr), mouseUVCoord,
                          ImVec2{rc.GetWidth(), rc.GetHeight()}, zoomSize);
  }
}

void InspectableImage::setUVs(ImVec2 leftTop, ImVec2 rightBottom) {
  uvLeftTop = leftTop;
  uvRightBottom = rightBottom;
}

void InspectableImage::setTexture(ImTextureID texId, std::span<const std::byte> rgbaData, std::size_t imgWidth) {
  textureId = texId;
  imageRGBAData = rgbaData;
  imageWidth = imgWidth;
}

void InspectableImage::setZoomSize(std::uint32_t zoom) { zoomSize = zoom; }
}  // namespace pf::ui::ig
