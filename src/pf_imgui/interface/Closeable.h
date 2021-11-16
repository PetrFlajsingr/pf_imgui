//
// Created by xflajs00 on 16.11.2021.
//

#ifndef PF_IMGUI_CLOSEABLE_H
#define PF_IMGUI_CLOSEABLE_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Observable_impl.h>

namespace pf::ui::ig {

/**
 * @brief Interface for closeable elements
 *
 * Provides a functionality to manage closing of the subclass
 */
class PF_IMGUI_EXPORT Closeable {
 public:
  virtual ~Closeable() = default;

  /**
    * Check if the Window can be closed with an X button in top right corner.
    * @return
    */
  [[nodiscard]] bool isCloseable() const;

  /**
    * Enable/disable user window closing using an X button in top right corner.
    * @param closeable
    */
  void setCloseable(bool closeable);

  /**
    * Add a listener for close event.
    *
    * This event is triggered when X button is clicked.
    * @param listener to be called on event
    * @return Subscription for listener cancellation
    */
  Subscription addCloseListener(std::invocable auto listener) { return closeObservableImpl.addListener(listener); }

 protected:
  void notifyClosed();

 private:
  bool closeable = false;
  Observable_impl<> closeObservableImpl;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_CLOSEABLE_H
