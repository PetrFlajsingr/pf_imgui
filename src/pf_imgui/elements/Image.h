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
class PF_IMGUI_EXPORT Image
    : public ItemElement,
      public Resizable,
      public Clickable,
      public ColorCustomizable<style::ColorOf::Button, style::ColorOf::ButtonHovered, style::ColorOf::ButtonActive,
                               style::ColorOf::NavHighlight, style::ColorOf::Border, style::ColorOf::BorderShadow> {
 public:
  /**
   * Provider of UV mapping for textures. First is left upper corner, right is right lower.
   */
  using UvMappingProvider = std::function<std::pair<ImVec2, ImVec2>()>;

  /**
   * @brief Struct for construction of Image.
   */
  struct Config {
    using Parent = Image;
    std::string_view name;    /*!< Unique name of the element */
    ImTextureID textureId;    /*!< Id of the texture to render */
    Size size = Size::Auto(); /*!< Size of the element */
    UvMappingProvider uvTextureMappingProvider = [] {
      return std::pair(ImVec2{0, 0}, ImVec2{1, 1});
    }; /*!< Provider of UV coordinates */
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
   * @param uvTextureMappingProvider provider of UV coordinates
   */
  Image(
      const std::string &elementName, ImTextureID imTextureId, Size size = Size::Auto(),
      UvMappingProvider uvTextureMappingProvider = [] {
        return std::pair(ImVec2{0, 0}, ImVec2{1, 1});
      });

  /**
   * Change texture ID.
   * @param imTextureId new id
   */
  void setTextureId(ImTextureID imTextureId);

 protected:
  void renderImpl() override;

 private:
  ImTextureID textureId;
  UvMappingProvider uvMappingProvider;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_ELEMENTS_IMAGE_H
