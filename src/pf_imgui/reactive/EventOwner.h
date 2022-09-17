//
// Created by xflajs00 on 15.09.2022.
//

#ifndef PF_IMGUI_REACTIVE_EVENTOWNER_H
#define PF_IMGUI_REACTIVE_EVENTOWNER_H

#include "Event.h"

namespace pf::ui::ig {

struct EventOwner {
 protected:
  template<typename ...Args>
  using Event = ClassEvent<EventOwner, Args...>;

  template<typename ...Args>
  static void Event_notify(Event<Args...> &event, Args &&...args) {
    event.notify(std::forward<Args>(args)...);
  }
};

}

#endif  //PF_IMGUI_REACTIVE_EVENTOWNER_H
