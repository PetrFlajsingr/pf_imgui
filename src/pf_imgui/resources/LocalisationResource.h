//
// Created by petr.flajsingr on 2/7/2022.
//

#ifndef PF_IMGUI_LOCALISATIONRESOURCE_H
#define PF_IMGUI_LOCALISATIONRESOURCE_H

#include "Resource.h"
#include <string>
#include <optional>

namespace pf::ui::ig {

class LocalisationResource : public Resource<std::string> {
 public:

  [[nodiscard]] const std::string &get() const override { return value; }

  void setLocale(const std::string &locale) {
    value = getValueForLocale(locale).value_or(key);
  }

  ~LocalisationResource() override = default;

 protected:
  virtual std::optional<std::string> getValueForLocale(std::string locale) = 0;

 private:
  std::string key;
  std::string value;
};

}

#endif  //PF_IMGUI_LOCALISATIONRESOURCE_H
