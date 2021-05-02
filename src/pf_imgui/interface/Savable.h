/**
 * @file Savable.h
 * @brief Interface for elements which can save their state.
 * @author Petr Flajšingr
 * @date 2.11.20
 */

#ifndef PF_IMGUI_INTERFACE_SAVABLEELEMENT_H
#define PF_IMGUI_INTERFACE_SAVABLEELEMENT_H

#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <string>
#include <toml++/toml.h>

namespace pf::ui::ig {

/**
 * @brief Interface for savable elements
 *
 * Provides a functionality to save subclasses' state and load it back up.
 */
class PF_IMGUI_EXPORT Savable {
 public:
  /**
   * Construct Savable with enabled/disabled saving capability
   * @param persistent
   */
  explicit Savable(Persistent persistent);

  Savable(Savable &&other) noexcept;
  Savable &operator=(Savable &&other) noexcept;

  /**
   *
   * @return
   */
  [[nodiscard]] bool isPersistent() const;

  /**
   * Load data from toml.
   * @param src source toml data
   */
  void unserialize(const toml::table &src);
  /**
   * Save data to toml.
   * @return toml::table if saving is enabled, nullopt otherwise
   */
  std::optional<toml::table> serialize();

  virtual ~Savable() = default;

 protected:
  /**
   * Method for subclasses' implementation of deserialization.
   * @param src source toml data
   */
  virtual void unserialize_impl(const toml::table &src) = 0;
  /**
   * Method for subclasses' implementation of serialization.
   * @return serialized toml data
   */
  virtual toml::table serialize_impl() = 0;

 private:
  bool persist;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_SAVABLEELEMENT_H
