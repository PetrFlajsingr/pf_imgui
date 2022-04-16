//
// Created by xflajs00 on 15.04.2022.
//

#ifndef PF_IMGUI_INTERFACE_TOMLSERIALIZABLE_H
#define PF_IMGUI_INTERFACE_TOMLSERIALIZABLE_H

#include <pf_imgui/_export.h>
#include <toml++/toml.h>

namespace pf::ui::ig {

/**
 * @todo Turn this into an interface which creates types fit for de/serialization by de/serializer interface
 */
struct PF_IMGUI_EXPORT TomlSerializable {
  /**
   * Save data to toml.
   * @return toml::table if saving is enabled, nullopt otherwise
   */
  [[nodiscard]] virtual toml::table toToml() const = 0;
  /**
   * Load data from toml.
   * @param src source toml data
   */
  virtual void setFromToml(const toml::table &src) = 0;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_INTERFACE_TOMLSERIALIZABLE_H
