/**
 * @file Image.h
 * @brief Image element.
 * @author Petr Flajšingr
 * @date 31.10.20
 */

#ifndef PF_IMGUI_ELEMENTS_IMAGE_H
#define PF_IMGUI_ELEMENTS_IMAGE_H

#include <functional>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Resizable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Image rendered from a texture. It can serve as a button.
 *
 * Image rendering should be handled by the use when descending from ImGuiInterface.
 */
class PF_IMGUI_EXPORT Image : public ItemElement, public Resizable, public Clickable {
 public:
  /**
   * @brief Struct for construction of Image.
   */
  struct Config { //-V730
    using Parent = Image;
    std::string_view name;    /*!< Unique name of the element */
    ImTextureID textureId;    /*!< Id of the texture to render */
    Size size = Size::Auto(); /*!< Size of the element */
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

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_IMAGE_H
