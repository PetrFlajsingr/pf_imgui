/**
 * @file Pin.h
 * @brief Pin for NodeEditor.
 * @author Petr Flajšingr
 * @date 27.3.22
 */

#ifndef PF_IMGUI_NODE_EDITOR_PIN_H
#define PF_IMGUI_NODE_EDITOR_PIN_H

#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_common/Explicit.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/node_editor/details/LinkPtrToRef.h>
#include <pf_imgui/reactive/Observable.h>
#include <range/v3/view/filter.hpp>

namespace pf::ui::ig {

// TODO: customization
// TODO: code link
/**
 * @brief A Pin to be placed inside Node. @see Node
 */
class Pin : public Renderable {
  friend class NodeEditor;
  friend class Node;
  /**
   * Get all Links in NodeEditor. It's done this way to avoid circular dependency.
   */
  [[nodiscard]] ranges::transform_view<ranges::ref_view<std::vector<std::unique_ptr<pf::ui::ig::Link>>>, details::LinkPtrToRef>
  getAllLinks();
  [[nodiscard]] ranges::transform_view<ranges::ref_view<const std::vector<std::unique_ptr<pf::ui::ig::Link>>>, details::LinkPtrToConstRef>
  getAllLinks() const;

 public:
  enum class Type { Input = static_cast<int>(ax::NodeEditor::PinKind::Input), Output = static_cast<int>(ax::NodeEditor::PinKind::Output) };

  /**
    * @brief Struct for construction of Node.
    */
  struct Config {
    using Parent = Pin;
    Explicit<std::string_view> name;  /*!< Unique name of the element */
    Explicit<std::string_view> label; /*!< Text rendered next to the pin */
  };
  /**
    * Construct Node
    * @param config construction args @see Node::Config
    */
  explicit Pin(Config &&config);
  /**
   * Construct Pin.
   * @param elementName unique name of the element
   * @param labelText text rendered next to the pin
   */
  Pin(std::string_view elementName, std::string_view labelText);

  /**
   * Get internally used id of the pin.
   */
  [[nodiscard]] ax::NodeEditor::PinId getId() const;

  /**
   * Get type of the pin (in/out).
   */
  [[nodiscard]] Type getType() const;

  /**
   * Get all currently connected Links.
   */
  [[nodiscard]] auto getLinks() {
    return getAllLinks() | ranges::views::filter([this](const Link &link) {
             return link.isValid() && (link.getInputPin().getId() == getId() || link.getOutputPin().getId() == getId());
           });
  }
  /**
   * Get all currently connected Links.
   */
  [[nodiscard]] auto getLinks() const {
    return getAllLinks() | ranges::views::filter([this](const Link &link) {
             return link.isValid() && (link.getInputPin().getId() == getId() || link.getOutputPin().getId() == getId());
           });
  }

  /**
   * Get Node which owns this Pin.
   */
  [[nodiscard]] Node &getNode();
  /**
   * Get Node which owns this Pin.
   */
  [[nodiscard]] const Node &getNode() const;

  /**
   * @return true if Pin has any active links
   */
  [[nodiscard]] bool hasAnyValidLinks() const;

  /**
   * Remove all Links, invalidating them.
   */
  void clearLinks();

  /**
   * @return true if this Pin accepts new links, false otherwise
   */
  [[nodiscard]] virtual bool acceptsNewLinks() const;
  /**
   * @return true if this Pin accepts connection with the other one
   */
  [[nodiscard]] virtual bool acceptsLinkWith([[maybe_unused]] Pin &other) const;

  /**
   * @return color used for a link which is in a valid configuration
   */
  [[nodiscard]] Color getValidLinkPreviewColor() const;
  /**
   * Set color used for a link which is in a valid configuration.
   */
  void setValidLinkPreviewColor(Color color);

  /**
   * @return thickness used for a link which is in a valid configuration
   */
  [[nodiscard]] float getValidLinkPreviewThickness() const;
  /**
   * Set thickness used for a link which is in a valid configuration.
   */
  void setValidLinkPreviewThickness(float thickness);

  /**
   * @return color used for a link which is in an invalid configuration
   */
  [[nodiscard]] Color getInvalidLinkPreviewColor() const;
  /**
   * Set color used for a link which is in an invalid configuration.
   */
  void setInvalidLinkPreviewColor(Color color);

  /**
   * @return thickness used for a link which is in an invalid configuration
   */
  [[nodiscard]] float getInvalidLinkPreviewThickness() const;
  /**
   * Set thickness used for a link which is in an invalid configuration.
   */
  void setInvalidLinkPreviewThickness(float thickness);

  /**
   * @return color used for a link which is not connected
   */
  [[nodiscard]] Color getUnconnectedLinkPreviewColor() const;
  /**
   * Set color used for a link which is not connected.
   */
  void setUnconnectedLinkPreviewColor(Color color);

  /**
   * @return thickness used for a link which is not connected
   */
  [[nodiscard]] float getUnconnectedLinkPreviewThickness() const;
  /**
   * Set thickness used for a link which is not connected.
   */
  void setUnconnectedLinkPreviewThickness(float thickness);

  /**
   * Create or get PopupMenu which is shown when the node is right clicked.
   */
  [[nodiscard]] PopupMenu &createOrGetPopupMenu();
  /**
   *
   * @return true if PopupMenu is active for this node, false otherwise
   */
  [[nodiscard]] bool hasPopupMenu() const;
  /**
   * Remove PopupMenu of this node, no lon
   */
  void removePopupMenu();

  Property<Label> label;

  ReadOnlyProperty<bool> hovered;

  Event<Link &, bool> linkChangedEvent;
  Event<> doubleClickEvent;

 protected:
  /**
   * @warning Do not override unless you completely want to change the way pins look
   */
  void renderImpl() override;

  /**
   * Render icon of the pin.
   */
  virtual void renderIcon();
  /**
   * Render information rendered next to the icon.
   */
  virtual void renderInfo();
  /**
   * Called when NodeEditor decides a Link is to be added
   */
  virtual void addLink(Link &link);

  void setHovered(bool newHovered);

  ax::NodeEditor::PinId id;
  Type type;

  Node *parent;
  // TODO: just make these public
  Color validLinkPreviewColor = Color::White;
  float validLinkPreviewThickness = 1.f;

  Color invalidLinkPreviewColor = Color::White;
  float invalidLinkPreviewThickness = 1.f;

  Color unconnectedLinkPreviewColor = Color::White;
  float unconnectedLinkPreviewThickness = 1.f;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_PIN_H
