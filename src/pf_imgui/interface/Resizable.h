//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H
#define PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H

#include <imgui.h>
#include <pf_imgui/_export.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Interface for resizable elements
 *
 * Provides a functionality to manage size for the subclass.
 *
 * @todo: observable part
 */
class PF_IMGUI_EXPORT Resizable {
 public:
  /**
   * Construct Resizable with the given size
   * @param size size
   */
  explicit Resizable(const ImVec2 &size);

  Resizable(Resizable &&other) noexcept;
  Resizable &operator=(Resizable &&other) noexcept;

  /**
   * Get current size.
   * @return current size
   */
  [[nodiscard]] const ImVec2 &getSize() const;
  /**
   * Set new size.
   * @param s new size
   */
  virtual void setSize(const ImVec2 &s);

  virtual ~Resizable() = default;

 private:
  ImVec2 size;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_INTERFACE_RESIZABLEELEMENT_H
