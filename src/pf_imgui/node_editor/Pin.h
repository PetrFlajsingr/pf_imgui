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
#include <pf_imgui/elements/PopupMenu.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Observable_impl.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

// TODO: customization
// TODO: link validation
// TODO: code link
// TODO: comments
// TODO: change the way links are handled so there doesn't have to be cleanup here nor in NodeEditor - maybe just through disconnect events
/**
 * @brief A Pin to be placed inside Node. @see Node
 */
class Pin : public Renderable, public Labellable {
  friend class NodeEditor;
  friend class Node;

 public:
  enum class Type {
    Input = static_cast<int>(ax::NodeEditor::PinKind::Input),
    Output = static_cast<int>(ax::NodeEditor::PinKind::Output)
  };

  /**
    * @brief Struct for construction of Node.
    */
  struct Config {
    using Parent = Pin;
    std::string name;  /*!< Unique name of the element */
    std::string label; /*!< Text rendered next to the pin */
  };
  /**
    * Construct Node
    * @param config construction args @see Node::Config
    */
  explicit Pin(Config &&config);
  /**
   * Construct Pin.
   * @param name unique name of the element
   * @param label text rendered next to the pin
   */
  Pin(const std::string &name, const std::string &label);
  ~Pin() override;

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
    return links | ranges::views::transform([](auto &link) -> Link & { return *link; });
  }
  /**
   * Get all currently connected Links.
   */
  [[nodiscard]] auto getLinks() const {
    return links | ranges::views::transform([](auto &link) -> const Link & { return *link; });
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
   * Add a listener for new link connections.
   * @param listener listener
   */
  Subscription addLinkListener(std::invocable<std::shared_ptr<Link>> auto &&listener) {
    return observableLink.addListener(std::forward<decltype(listener)>(listener));
  }

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
  [[nodiscard]] const ImVec4 &getValidLinkPreviewColor() const;
  /**
   * Set color used for a link which is in a valid configuration.
   */
  void setValidLinkPreviewColor(const ImVec4 &color);

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
  [[nodiscard]] const ImVec4 &getInvalidLinkPreviewColor() const;
  /**
   * Set color used for a link which is in an invalid configuration.
   */
  void setInvalidLinkPreviewColor(const ImVec4 &color);

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
  [[nodiscard]] const ImVec4 &getUnconnectedLinkPreviewColor() const;
  /**
   * Set color used for a link which is not connected.
   */
  void setUnconnectedLinkPreviewColor(const ImVec4 &color);

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

 protected:
  constexpr static int LINK_CLEANUP_FREQUENCY = 100;
  int sinceLastLinkCleanup = LINK_CLEANUP_FREQUENCY;

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
   * Called when NodeEditor decides a Lin
   */
  virtual void addLink(std::shared_ptr<Link> link);

 private:
  ax::NodeEditor::PinId id;
  Type type;

  Node *parent;

  ImVec4 validLinkPreviewColor = ImVec4(1, 1, 1, 1);
  float validLinkPreviewThickness = 1.f;

  ImVec4 invalidLinkPreviewColor = ImVec4(1, 1, 1, 1);
  float invalidLinkPreviewThickness = 1.f;

  ImVec4 unconnectedLinkPreviewColor = ImVec4(1, 1, 1, 1);
  float unconnectedLinkPreviewThickness = 1.f;

  std::vector<std::shared_ptr<Link>> links;
  Observable_impl<std::shared_ptr<Link>> observableLink;

  std::unique_ptr<PopupMenu> popupMenu = nullptr;
};

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_NODE_EDITOR_PIN_H
