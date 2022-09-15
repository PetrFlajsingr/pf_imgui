//
// Created by xflajs00 on 15.09.2022.
//

#ifndef PF_IMGUI_REACTIVE_PROPERTYOWNER_H
#define PF_IMGUI_REACTIVE_PROPERTYOWNER_H

#include "Observable.h"

namespace pf::ui::ig {

template<typename T>
struct PropertyOwner {
  template<typename U>
  using ReadOnlyProperty = ObservableProperty<T, U, ReadOnlyTag>;
  template<typename U>
  using Property = ObservableProperty<T, U, ReadWriteTag>;

 protected:
  /** Provides access to transaction - which may be private - in derived classes */
  template<typename U, OneOf<ReadOnlyTag, ReadWriteTag> Tag>
  typename ObservableProperty<T, U, Tag>::Transaction modifyProperty(ObservableProperty<T, U, Tag> &property) {
    return property.modify();
  }
  /** Provides acces to inner value of a property */
  template<typename U, OneOf<ReadOnlyTag, ReadWriteTag> Tag>
  typename ObservableProperty<T, U, Tag>::reference getPropertyValueRef(ObservableProperty<T, U, Tag> &property) {
    return property.value;
  }
  /** Allows triggering of listeners, when internal value is changed without transaction */
  template<typename U, OneOf<ReadOnlyTag, ReadWriteTag> Tag>
  void triggerPropertyListeners(ObservableProperty<T, U, Tag> &property) {
    return property.triggerListeners();
  }
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_REACTIVE_PROPERTYOWNER_H
