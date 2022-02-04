/**
* @file Clickable.h
* @brief Interface for elements which can be clicked.
* @author Petr Flajšingr
* @date 14.11.20
*/

#ifndef PF_IMGUI_INTERFACE_CLICKABLE_H
#define PF_IMGUI_INTERFACE_CLICKABLE_H

#include "Observable_impl.h"
#include <functional>
#include <pf_common/Subscription.h>
#include <pf_imgui/_export.h>
#include <unordered_map>

namespace pf::ui::ig {

/**
* @brief Interface for clickable elements
*
* Provides the ability to add multiple observers and also remove them via Subscription.
*/
class PF_IMGUI_EXPORT Clickable {
public:
 /**
  * Add a parameterless listener, which is called every time the item is clicked.
  * @param fnc listener for click event
  * @return instance of Subscription, which allows to unsubscribe the listener
  * @see Subscription
  */
 Subscription addClickListener(std::invocable auto &&fnc) {
   return observableImpl.addListener(std::forward<decltype(fnc)>(fnc));
 }

 virtual ~Clickable() = default;

protected:
 /**
  * Call all listeners.
  */
 void notifyOnClick();

private:
 Observable_impl<> observableImpl;
};

}// namespace pf::ui::ig

#endif//PF_IMGUI_INTERFACE_CLICKABLE_H
