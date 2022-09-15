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
#include <pf_imgui/common/Size.h>
#include <pf_imgui/common/Texture.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/reactive/Observable.h>
#include <span>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Image rendered from a texture. For button version see ImageButton. @see ImageButtom
 *
 * Image rendering should be handled by the user when deriving from ImGuiInterface.
 */
class PF_IMGUI_EXPORT Image : public ItemElement {
 public:
  /**
   * @brief Struct for construction of Image.
   */
  struct Config {
    using Parent = Image;
    Explicit<std::string_view> name;            /*!< Unique name of the element */
    Explicit<std::shared_ptr<Texture>> texture; /*!< Texture to render */
    Size size = Size::Auto();                   /*!< Size of the element */
  };
  /**
   * Construct Image
   * @param config construction args @see Image::Config
   */
  explicit Image(Config &&config);
  /**
   * Construct Image.
   * @param elementName ID of the element
   * @param tex texture to render
   * @param initialSize size of the image on screen
   */
  Image(std::string_view elementName, std::shared_ptr<Texture> tex, Size initialSize = Size::Auto());

  /**
   * Change texture ID.
   * @param imTextureId new id
   */
  void setTexture(std::shared_ptr<Texture> tex);
  [[nodiscard]] const std::shared_ptr<Texture> &getTexture() const;
  /**
   * Set UVs according to which the image will get sampled.
   */
  void setUVs(ImVec2 leftTop, ImVec2 rightBottom);

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  std::shared_ptr<Texture> texture;
  ImVec2 uvLeftTop{0.f, 0.f};
  ImVec2 uvRightBottom{1.f, 1.f};
};

/**
 * @brief Image rendered from a texture, has a special tooltip zooming on pixels and providing info.
 *
 * Image rendering should be handled by the user when deriving from ImGuiInterface.
 */
class PF_IMGUI_EXPORT InspectableImage : public ItemElement {
 public:
  /**
   * @brief Trigger for inspection popup.
   */
  enum class Trigger { Hover, LeftMouseDown, RightMouseDown };
  /**
   * @brief Construction args for @see InspectableImage.
   */
  struct Config {
    using Parent = InspectableImage;
    Explicit<std::string_view> name;            /*!< Unique name of the element */
    Explicit<std::shared_ptr<Texture>> texture; /*!< Texture to render */
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
   * @param tex texture
   */
  InspectableImage(std::string_view elementName, Size s, std::span<const std::byte> rgbaData, std::size_t imgWidth,
                   std::shared_ptr<Texture> tex, Trigger trigger);
  /**
   * Set UVs according to which the image will get sampled.
   */
  void setUVs(ImVec2 leftTop, ImVec2 rightBottom);
  /**
   * Set texture along with necessary information.
   * @param tex texture
   * @param rgbaData image data, the element won't store this, you needto keep the memory valid
   * @param imgWidth width of the provided image
   */
  void setTexture(std::shared_ptr<Texture> tex, std::span<const std::byte> rgbaData, std::size_t imgWidth);

  /**
   * Set amount of pixels zoomed in around selection.
   */
  void setZoomSize(std::uint32_t zoom);

  Observable<Size> size;

 protected:
  void renderImpl() override;

 private:
  std::span<const std::byte> imageRGBAData;
  int zoomSize = 20;
  std::size_t imageWidth;
  std::shared_ptr<Texture> texture;
  ImVec2 uvLeftTop{0.f, 0.f};
  ImVec2 uvRightBottom{1.f, 1.f};
  Trigger trig;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_IMAGE_H
