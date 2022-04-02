//
// Created by xflajs00 on 02.04.2022.
//

#ifndef PF_IMGUI_COMMENT_H
#define PF_IMGUI_COMMENT_H

#include <pf_imgui/interface/Renderable.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Resizable.h>
#include <imgui_node_editor.h>

namespace pf::ui::ig {

class Comment : public Renderable, public Labellable {
 public:
  Comment(const std::string &name, const std::string &label, Size initSize, ax::NodeEditor::NodeId commentId);

  [[nodiscard]] ax::NodeEditor::NodeId getId() const;

 protected:
  void renderImpl() override;

 private:
  ax::NodeEditor::NodeId id;
  ImVec2 size;
};
}  // namespace pf::ui::ig

#endif  //PF_IMGUI_COMMENT_H
