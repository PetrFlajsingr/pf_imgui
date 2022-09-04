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
#include <pf_imgui/Label.h>
#include <pf_imgui/Position.h>
#include <pf_imgui/Size.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/reactive/Observable.h>
#include <pf_imgui/Size.h>

namespace pf::ui::ig {

/**
 * @brief Comment which can contain nodes and move them around
 * TODO: ?detect what nodes are inside?
 */
class Comment : public NodeBase {
  friend class NodeEditor;

 public:
  /**
   * Construct Comment
   * @param elementName unique name of the element
   * @param labelText text rendered in header of the Comment
   * @param initialSize initial size of the Comment area
   */
  Comment(const std::string &elementName, const std::string &labelText, Size initialSize);
  Comment(const std::string &elementName, const Position &initPosition, const std::string &labelText, const Size &s);

  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;

  bool sizeDirty = false;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COMMENT_H
