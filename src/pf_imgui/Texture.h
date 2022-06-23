/**
 * @file Texture.h
 * @brief Type representing a texture in pf_imgui.
 * @author Petr Flajšingr
 * @date 22.6.22
 */

#ifndef PF_IMGUI_TEXTURE_H
#define PF_IMGUI_TEXTURE_H

#include <imconfig.h>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

/**
 * @brief A texture used for image rendering.
 */
class PF_IMGUI_EXPORT Texture {
 public:
  virtual ~Texture() = default;
  [[nodiscard]] virtual ImTextureID getID() const = 0;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_TEXTURE_H
