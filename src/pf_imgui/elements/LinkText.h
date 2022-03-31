/**
* @file LinkText.h
* @brief Text element serving as an URL.
* @author Petr Flajšingr
* @date 14.02.22
*/

#ifndef PF_IMGUI_LINKTEXT_H
#define PF_IMGUI_LINKTEXT_H

#include <functional>
#include <pf_imgui/interface/Clickable.h>
#include <pf_imgui/interface/ItemElement.h>
#include <pf_imgui/interface/Labellable.h>
#include <string>
#include <utility>

namespace pf::ui::ig {

/**
 * @brief Text element providing click detection.
 */
class LinkText : public ItemElement, public Labellable, public Clickable {
 public:
  using LinkClickHandler = std::function<void(std::string_view)>;
  /**
   * @brief Config for LinkText construction
   */
  struct Config {
    using Parent = LinkText;
    std::string_view name;         /*!< Unique name of the element */
    std::string_view label;        /*!< Text of the element */
    std::string link;              /*!< Underlying link/url */
    LinkClickHandler clickHandler; /*!< Handler for url interact */
  };
  /**
   * Construct LinkText
   * @param config construction args @see LinkText::Config
   */
  explicit LinkText(Config &&config);

  /**
   * Construct LinkText
   * @param elementName unique name of the element
   * @param label text of the element
   * @param link underlying link/url
   * @param linkClickHandler handler for url interact
   */
  LinkText(const std::string &elementName, const std::string &label, std::string link,
           std::invocable<std::string_view> auto &&linkClickHandler)
      : ItemElement(elementName), Labellable(label), link(std::move(link)),
        linkClickHandler(std::forward<decltype(linkClickHandler)>(linkClickHandler)) {
    setTooltip(LinkText::link);
  }

 protected:
  void renderImpl() override;

 private:
  std::string link;
  LinkClickHandler linkClickHandler;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_LINKTEXT_H
