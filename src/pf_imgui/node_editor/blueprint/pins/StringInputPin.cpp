//
// Created by xflajs00 on 29.03.2022.
//

#include "StringInputPin.h"

namespace pf::ui::ig::bp {

/*
struct InputConfig {
  Width width;
  std::string value{};
  TextInputType inputType = TextInputType::SingleLine;
  std::size_t maxInputLength = 256;
  TextTrigger eventTrigger = TextTrigger::Character;
  Flags<TextFilter> filters = TextFilter::None;
};
 */

StringInputPin::StringInputPin(const std::string &elementName, const std::string &labelText, Color pinColor,
                               StringInputPin::InputConfig &&config)
    : InteractablePin<InputText>(elementName, labelText, pinColor, config.width,
                                 CreateInputConfig(uniqueId(), labelText, std::move(config))) {}

InputText::Config StringInputPin::CreateInputConfig(const std::string &name, const std::string &label,
                                                    StringInputPin::InputConfig &&config) {
  return {.name = name,
          .label = label,
          .value = config.value,
          .inputType = config.inputType,
          .maxInputLength = config.maxInputLength,
          .eventTrigger = config.eventTrigger};
}

std::unique_ptr<StringInputPin> StringInputPin::ConstructFromToml(ig::Node *parent, const toml::table &src) {
  auto result = std::make_unique<StringInputPin>("", "", Color::White, InputConfig{0.f});
  result->parent = parent;
  result->setFromToml(src);
  return result;
}

toml::table StringInputPin::toToml() const {
  auto result = InteractablePin::toToml();
  if (auto data = result.find("data"); data != result.end()) {
    if (auto table = data->second.as_table(); table != nullptr) {
      table->insert_or_assign("inputType", magic_enum::enum_name(inputElement->getInputType()));
      table->insert_or_assign("eventTrigger", magic_enum::enum_name(inputElement->getInputTrigger()));
      table->insert_or_assign("filters", *inputElement->getFilters());
    }
  }
  return result;
}

void StringInputPin::setFromToml(const toml::table &src) {
  InteractablePin::setFromToml(src);
  if (auto data = src.find("data"); data != src.end()) {
    if (auto dataTable = data->second.as_table(); dataTable != nullptr) {
      if (auto inputType = dataTable->find("inputType"); inputType != dataTable->end()) {
        if (auto inputTypeValue = inputType->second.as_string(); inputTypeValue != nullptr) {
          if (auto inputTypeEnum = magic_enum::enum_cast<TextInputType>(inputTypeValue->get());
              inputTypeEnum.has_value()) {
            InteractablePin::inputElement->setInputType(inputTypeEnum.value());
          }
        }
      }
      if (auto eventTrigger = dataTable->find("eventTrigger"); eventTrigger != dataTable->end()) {
        if (auto eventTriggerValue = eventTrigger->second.as_string(); eventTriggerValue != nullptr) {
          if (auto eventTriggerEnum = magic_enum::enum_cast<TextTrigger>(eventTriggerValue->get());
              eventTriggerEnum.has_value()) {
            InteractablePin::inputElement->setInputTrigger(eventTriggerEnum.value());
          }
        }
      }
      if (auto filters = dataTable->find("filters"); filters != dataTable->end()) {
        if (auto filtersValue = filters->second.as_integer(); filtersValue != nullptr) {
          InteractablePin::inputElement->setFilters(Flags<TextFilter>{static_cast<TextFilter>(filtersValue->get())});
        }
      }
    }
  }
}

}  // namespace pf::ui::ig::bp