/**
 * @file Image.h
 * @brief Image element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_IMAGE_H
#define PF_IMGUI_ELEMENTS_IMAGE_H

#include <functional>
#include <pf_common/Explicit.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Image rendered from a texture. For button version see ImageButton. @see ImageButtom
 *
 * Image rendering should be handled by the user when deriving from ImGuiInterface.
 */
class PF_IMGUI_EXPORT Image : public ItemElement, public Resizable {
 public:
  /**
   * @brief Struct for construction of Image.
   */
  struct Config {
    using Parent = Image;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<ImTextureID> textureId; /*!< Id of the texture to render */
    Size size = Size::Auto();        /*!< Size of the element */
  };
  /**
   * Construct Image
   * @param config construction args @see Image::Config
   */
  explicit Image(Config &&config);
  /**
   * Construct Image.
   * @param elementName ID of the element
   * @param imTextureId ID of a texture
   * @param size size of the image on screen
   */
  Image(const std::string &elementName, ImTextureID imTextureId, Size size = Size::Auto());

  /**
   * Change texture ID.
   * @param imTextureId new id
   */
  void setTextureId(ImTextureID imTextureId);
  [[nodiscard]] ImTextureID getTextureId() const;
  /**
   * Set UVs according to which the image will get sampled.
   */
  void setUVs(ImVec2 leftTop, ImVec2 rightBottom);

 protected:
  void renderImpl() override;

 private:
  ImTextureID textureId;
  ImVec2 uvLeftTop{0.f, 0.f};
  ImVec2 uvRightBottom{1.f, 1.f};
};

/**
 * @brief Image rendered from a texture, has a special tooltip zooming on pixels and providing info.
 *
 * Image rendering should be handled by the user when deriving from ImGuiInterface.
 */
class PF_IMGUI_EXPORT InspectableImage : public ItemElement, public Resizable {
 public:
  /**
   * @brief Trigger for inspection popup.
   */
  enum class Trigger {
    Hover, LeftMouseDown, RightMouseDown
  };
  /**
   * @brief Construction args for @see InspectableImage.
   */
  struct Config {
    using Parent = InspectableImage;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Explicit<ImTextureID> textureId; /*!< Id of the texture to render */
    Explicit<std::span<const std::byte>>
        rgbaData;                     /*!< Image data, the element won't store this, you needto keep the memory valid */
    Explicit<std::size_t> imageWidth; /*!< Width of the provided image */
    Size size = Size::Auto();         /*!< Size of the element */
    Trigger trigger = Trigger::Hover; /*!< Trigger for inspect overlay popup */
  };
  /**
   * Construct InspectableImage.
   * @param config construction args @see InspectableImage::Config
   */
  explicit InspectableImage(Config &&config);
  /**
   * Construct InspectableImage.
   * @param elementName unique name of the element
   * @param s size of the element
   * @param rgbaData image data, the element won't store this, you needto keep the memory valid
   * @param imgWidth width of the provided image
   * @param texId texture id
   */
  InspectableImage(const std::string &elementName, Size s, std::span<const std::byte> rgbaData, std::size_t imgWidth,
                   ImTextureID texId, Trigger trigger);
  /**
   * Set UVs according to which the image will get sampled.
   */
  void setUVs(ImVec2 leftTop, ImVec2 rightBottom);
  /**
   * Set texture along with necessary information.
   * @param texId texture id
   * @param rgbaData image data, the element won't store this, you needto keep the memory valid
   * @param imgWidth width of the provided image
   */
  void setTexture(ImTextureID texId, std::span<const std::byte> rgbaData, std::size_t imgWidth);

  /**
   * Set amount of pixels zoomed in around selection.
   */
  void setZoomSize(std::uint32_t zoom);

 protected:
  void renderImpl() override;

 private:
  std::span<const std::byte> imageRGBAData;
  int zoomSize = 20;
  std::size_t imageWidth;
  ImTextureID textureId;
  ImVec2 uvLeftTop{0.f, 0.f};
  ImVec2 uvRightBottom{1.f, 1.f};
  Trigger trig;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_IMAGE_H
