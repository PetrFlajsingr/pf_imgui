//
// Created by Petr on 6/22/2022.
//

#ifndef PF_IMGUI_TEXTURE_H
#define PF_IMGUI_TEXTURE_H

#include <pf_imgui/_export.h>
#include <imconfig.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT Texture {
 public:
  virtual ~Texture() = default;
  [[nodiscard]] virtual ImTextureID getID() const = 0;
};

}

#endif  //PF_IMGUI_TEXTURE_H
