//
// Created by xflajs00 on 17.04.2022.
//

#ifndef PF_IMGUI_ELEMENTS_CUSTOMICONBUTTONBEHAVIOR_H
#define PF_IMGUI_ELEMENTS_CUSTOMICONBUTTONBEHAVIOR_H

#include <pf_imgui/common/Color.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Savable.h>
#include <pf_imgui/interface/ValueContainer.h>
#include <pf_imgui/reactive/Event.h>
#include <pf_imgui/style/ColorPalette.h>

namespace pf::ui::ig {

class CustomIconButtonBehavior : public ItemElement {
 protected:
  enum class State { None, Hovered, MouseDown, Clicked };

 public:
  explicit CustomIconButtonBehavior(std::string_view elementName);
  [[nodiscard]] Color getIconColor() const;
  void setIconColor(Color newColor);

  ColorPalette<ColorOf::Button, ColorOf::ButtonActive, ColorOf::ButtonHovered> color;

 protected:
  virtual void renderIcon(ImDrawList *drawList, ImVec2 cursorPos) = 0;
  virtual void update(State state) = 0;

  [[nodiscard]] Size getSize() const;
  void setSize(Size newSize);

  void renderImpl() override;

  [[nodiscard]] Color getBackgroundColor() const;

  bool keepHighlighted = false;

 private:
  Color iconColor = Color::White;
  Color backgroundColor = Color::White;
  Size size{22, 22};
};

class CustomIconButton : public CustomIconButtonBehavior {
 public:
  explicit CustomIconButton(std::string_view elementName);

  Event<> clickEvent;

 protected:
  void update(State state) override;
  void notifyClickEvent();
};

class CustomIconToggle : public CustomIconButtonBehavior, public ValueContainer<bool>, public Savable {
 public:
  explicit CustomIconToggle(std::string_view elementName, bool initValue = false,
                            Persistent persistent = Persistent::No);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  Property<bool> checked;

  [[nodiscard]] const bool &getValue() const override;
  void setValue(const bool &newValue) override;

 protected:
  Subscription addValueListenerImpl(std::function<void(const bool &)> listener) override;

  void update(State state) override;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_CUSTOMICONBUTTONBEHAVIOR_H
