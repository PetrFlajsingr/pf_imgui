/**
 * @file Renderable.h
 * @brief Base class for renderables.
 * @author Petr Flajšingr
 * @date 23.1.21
 */

#ifndef PF_IMGUI_INTERFACE_RENDERABLE_H
#define PF_IMGUI_INTERFACE_RENDERABLE_H

#include <pf_common/enums.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/enums.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Interface for all renderables
 *
 * Implements basic properties of renderable objects: visibility, enable/disable. It also identifies an object with a unique name.
 *
 * @warning If there are duplicate IDs in the render tree the program may have some undefined behavior for functions which use IDs.
 */
class PF_IMGUI_EXPORT Renderable {
 public:
  /**
   * Create Renderable with unique ID
   * @param name unique ID for object
   */
  explicit Renderable(std::string name);
  Renderable(Renderable &&other) noexcept;
  Renderable &operator=(Renderable &&other) noexcept;
  Renderable(const Renderable &) = delete;
  Renderable &operator=(const Renderable &) = delete;

  /**
   * Render this object.
   *
   * @warning DO NOT OVERRIDE UNLESS YOU WANT TO LOSE SOME FUNCTIONALITY
   * @details If you override this function you should base the new implementation on this one, so that no
   * expected functionality is lost.
   */
  virtual void render();

  virtual ~Renderable() = default;

  /**
   * Get current visibility.
   * @details When an object is invisible it is no longer rendered.
   * @return visibility of the renderable
   */
  [[nodiscard]] Visibility getVisibility() const;
  /**
   * Set new visibility.
   * @details When an object is invisible it is no longer rendered.
   * @param visibility new visibility
   */
  void setVisibility(Visibility visibility);

  /**
   * Get current enabled state.
   * @details When an object is not enabled it is still rendered, but it doesn't accept any user input. It is visually distinguished.
   * @return enabled state
   */
  [[nodiscard]] Enabled getEnabled() const;
  /**
   * Set enabled state.
   * @details When an object is not enabled it is still rendered, but it doesn't accept any user input. It is visually distinguished.
   * @param eleState new enabled state
   */
  void setEnabled(Enabled eleState);

  /**
   * Unique name of an element.
   * @return
   */
  [[nodiscard]] const std::string &getName() const;

 protected:
  /**
   * Implementation of rendering without checks for visibility etc. This should do pure rendering, nothing else.
   */
  virtual void renderImpl() = 0;

 private:
  Visibility visibility = Visibility::Visible;
  Enabled enabled = Enabled::Yes;
  std::string name;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_RENDERABLE_H
