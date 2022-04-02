/**
* @file Comment.h
* @brief Commet for NodeEditor.
* @author Petr Flajšingr
* @date 2.4.22
*/

#ifndef PF_IMGUI_COMMENT_H
#define PF_IMGUI_COMMENT_H

#include <imgui_node_editor.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Positionable.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Resizable.h>

namespace pf::ui::ig {

/**
 * @brief Comment which can contain nodes and move them around
 * TODO: removal of the comment + events
 * TODO: ?detect what nodes are inside?
 * TODO: popup menu
 */
class Comment : public Renderable, public Labellable {
 public:
  /**
   * Construct Comment
   * @param name unique name of the element
   * @param label text rendered in header of the Comment
   * @param initSize initial size of the Comment area
   * @param commentId unique internal id
   */
  Comment(const std::string &name, const std::string &label, Size initSize, ax::NodeEditor::NodeId commentId);

  /**
   *
   * @return unique internal id
   */
  [[nodiscard]] ax::NodeEditor::NodeId getId() const;

  /**
   * Get node position within editor.
   * @return position within editor
   */
  [[nodiscard]] Position getPosition() const;
  /**
   * Set node position within editor.
   * @param position new position
   */
  void setPosition(Position position);

  /**
   * Get Node's size.
   * @return size
   */
  [[nodiscard]] Size getSize() const;

 protected:
  void renderImpl() override;

 private:
  ax::NodeEditor::NodeId id;
  ImVec2 size;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COMMENT_H
