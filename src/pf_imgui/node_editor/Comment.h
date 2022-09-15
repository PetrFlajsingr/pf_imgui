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
#include <pf_imgui/common/Label.h>
#include <pf_imgui/common/Position.h>
#include <pf_imgui/common/Size.h>
#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/reactive/Observable.h>

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
  Comment(std::string_view elementName, std::string_view labelText, Size initialSize);
  Comment(std::string_view elementName, Position initPosition, std::string_view labelText, Size s);

  Observable<Label> label;

  Observable<Size> size;

 protected:
  void renderImpl() override;

  bool sizeDirty = false;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COMMENT_H
