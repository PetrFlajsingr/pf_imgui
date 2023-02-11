//
// Created by xflajs00 on 15.09.2022.
//

#ifndef PF_IMGUI_REACTIVE_PROPERTYOWNER_H
#define PF_IMGUI_REACTIVE_PROPERTYOWNER_H

#include "Observable.h"

namespace pf::ui::ig {

struct PropertyOwner {
 protected:
  template<typename U, ObservableChangeDetector<U> Detector = DefaultChangeDetector<U>>
  using ReadOnlyProperty = ObservableProperty<PropertyOwner, U, ReadOnlyTag, Detector>;
  template<typename U, ObservableChangeDetector<U> Detector = DefaultChangeDetector<U>>
  using Property = ObservableProperty<PropertyOwner, U, ReadWriteTag, Detector>;
  /** Provides access to transaction - which may be private - in derived classes */
  template<typename U, OneOf<ReadOnlyTag, ReadWriteTag> Tag, ObservableChangeDetector<U> Detector>
  static typename ObservableProperty<PropertyOwner, U, Tag, Detector>::Transaction
  Prop_modify(ObservableProperty<PropertyOwner, U, Tag, Detector> &property) {
    return property.modify();
  }
  /** Provides access to inner value of a property */
  template<typename U, OneOf<ReadOnlyTag, ReadWriteTag> Tag, ObservableChangeDetector<U> Detector>
  static typename ObservableProperty<PropertyOwner, U, Tag, Detector>::reference
  Prop_value(ObservableProperty<PropertyOwner, U, Tag, Detector> &property) {
    return property.value;
  }
  /** Allows triggering of listeners, when internal value is changed without transaction */
  template<typename U, OneOf<ReadOnlyTag, ReadWriteTag> Tag, ObservableChangeDetector<U> Detector>
  static void Prop_triggerListeners(ObservableProperty<PropertyOwner, U, Tag, Detector> &property) {
    return property.triggerListeners();
  }
};

#define PF_IMGUI_PROPERTY_OWNER_ALIASES(template_typename)                                                                                 \
  template<typename template_typename, ObservableChangeDetector<template_typename> Detector = DefaultChangeDetector<U>>                    \
  using ReadOnlyProperty = PropertyOwner::ReadOnlyProperty<template_typename, Detector>;                                                   \
  template<typename template_typename, ObservableChangeDetector<template_typename> Detector = DefaultChangeDetector<U>>                    \
  using Property = PropertyOwner::Property<template_typename, Detector>;

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_REACTIVE_PROPERTYOWNER_H
