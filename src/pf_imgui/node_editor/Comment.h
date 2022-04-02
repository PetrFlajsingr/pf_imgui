/**
* @file Comment.h
* @brief Commet for NodeEditor.
* @author Petr Flajšingr
* @date 2.4.22
*/

#ifndef PF_IMGUI_COMMENT_H
#define PF_IMGUI_COMMENT_H

#include "NodeBase.h"
#include "fwd.h"
#include <imgui_node_editor.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/Position.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/Size.h>

namespace pf::ui::ig {

/**
 * @brief Comment which can contain nodes and move them around
 * TODO: ?detect what nodes are inside?
 */
class Comment : public NodeBase, public Labellable {
  friend class NodeEditor;

 public:
  /**
   * Construct Comment
   * @param name unique name of the element
   * @param label text rendered in header of the Comment
   * @param initSize initial size of the Comment area
   * @param commentId unique internal id
   */
  Comment(const std::string &name, const std::string &label, Size initSize);

 protected:
  void renderImpl() override;

 private:
  ImVec2 size;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COMMENT_H