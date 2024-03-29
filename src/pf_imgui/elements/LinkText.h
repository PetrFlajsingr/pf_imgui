/**
 * @file LinkText.h
 * @brief Text element serving as an URL.
 * @author Petr Flajšingr
 * @date 14.02.22
 */

#ifndef PF_IMGUI_ELEMENTS_LINKTEXT_H
#define PF_IMGUI_ELEMENTS_LINKTEXT_H

#include <functional>
#include <pf_common/Explicit.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/ItemElement.h>
#include <utility>

namespace pf::ui::ig {
// TODO: styles
/**
 * @brief Text element providing click detection.
 */
class LinkText : public ItemElement {
 public:
  using LinkClickHandler = std::function<void(std::string_view)>;
  /**
   * @brief Config for LinkText construction
   */
  struct Config {
    using Parent = LinkText;
    Explicit<std::string_view> name;         /*!< Unique name of the element */
    Explicit<std::string_view> label;        /*!< Text of the element */
    Explicit<std::string> link;              /*!< Underlying link/url */
    Explicit<LinkClickHandler> clickHandler; /*!< Handler for url interact */
  };
  /**
   * Construct LinkText
   * @param config construction args @see LinkText::Config
   */
  explicit LinkText(Config &&config);

  /**
   * Construct LinkText
   * @param elementName unique name of the element
   * @param labelText text of the element
   * @param linkStr underlying link/url
   * @param linkClickHandlerFnc handler for url interact
   */
  LinkText(std::string_view elementName, std::string_view labelText, std::string linkStr,
           std::invocable<std::string_view> auto &&linkClickHandlerFnc)
      : ItemElement(elementName), label(std::string{labelText}), link(std::move(linkStr)),
        linkClickHandler(std::forward<decltype(linkClickHandlerFnc)>(linkClickHandlerFnc)) {
    setTooltip(LinkText::link);
  }

  Font font = Font::Default();
  Property<Label> label;

  Event<> clickEvent;

 protected:
  void renderImpl() override;

 private:
  std::string link;
  LinkClickHandler linkClickHandler;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_LINKTEXT_H
