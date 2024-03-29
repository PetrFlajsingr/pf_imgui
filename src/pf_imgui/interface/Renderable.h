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
#include <pf_imgui/reactive/EventOwner.h>
#include <pf_imgui/reactive/PropertyOwner.h>
#include <string_view>

namespace pf::ui::ig {

/**
 * @brief Interface for all renderables
 *
 * Implements basic properties of renderable objects: visibility, enable/disable. It also identifies an object with a unique name.
 *
 * @warning If there are duplicate IDs in the render tree the program may have some undefined behavior for functions which use IDs.
 */
class PF_IMGUI_EXPORT Renderable : protected PropertyOwner, protected EventOwner {
 public:
  /**
   * Create Renderable with unique ID
   * @param renderableName unique ID for object
   */
  explicit Renderable(std::string_view renderableName);
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
   * Unique name of an element.
   * @return
   */
  [[nodiscard]] const std::string &getName() const;

  Property<Visibility> visibility{Visibility::Visible};
  Property<bool> enabled{true};

 protected:
  /**
   * Implementation of rendering without checks for visibility etc. This should do pure rendering, nothing else.
   */
  virtual void renderImpl() = 0;

  void setName(std::string newName);

 private:
  std::string name;
};

}  // namespace pf::ui::ig

#endif  // PF_IMGUI_INTERFACE_RENDERABLE_H
