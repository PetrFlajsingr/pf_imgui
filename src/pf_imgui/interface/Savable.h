/**
 * @file Savable.h
 * @brief Interface for elements which can save their state.
 * @author Petr Flajšingr
 * @date 2.11.20
 */

#ifndef PF_IMGUI_INTERFACE_SAVABLE_H
#define PF_IMGUI_INTERFACE_SAVABLE_H

#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <pf_imgui/interface/TomlSerializable.h>
#include <toml++/toml.h>

namespace pf::ui::ig {

/**
 * @brief Interface for savable elements
 *
 * Provides a functionality to save subclasses' state and load it back up.
 */
class PF_IMGUI_EXPORT Savable : public TomlSerializable {
 public:
  /**
   * Construct Savable with enabled/disabled saving capability
   * @param persistent
   */
  explicit Savable(Persistent persistent);
  virtual ~Savable() = default;

  Savable(Savable &&other) noexcept;
  Savable &operator=(Savable &&other) noexcept;

  /**
   * @return true if the object is marked as persistent, thus saving its data to disk
   */
  [[nodiscard]] bool isPersistent() const;

  void setPersistent(bool persistent);

 private:
  bool persist;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_INTERFACE_SAVABLE_H
