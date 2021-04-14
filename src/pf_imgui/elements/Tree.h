//
// Created by petr on 10/31/20.
//

#ifndef PF_IMGUI_IMGUI_ELEMENTS_TREE_H
#define PF_IMGUI_IMGUI_ELEMENTS_TREE_H

#include <pf_imgui/_export.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Labellable.h>
#include <string>

namespace pf::ui::ig {

/**
 * @brief Structure for organising UI elements in a tree.
 *
 * Each node can contain subnodes or any other elements.
 */
class PF_IMGUI_EXPORT Tree : public Element, public Labellable, public ElementContainer {
 public:
  /**
   * Construct Tree.
   * @param elementName ID of the node
   * @param label text rendered on the node
   */
  Tree(const std::string &elementName, const std::string &label);

  /**
   * Create a new subnode.
   * @param elementName ID of the node
   * @param caption text rendered on the node
   * @return reference to the newly created node
   */
  Tree &addNode(const std::string &elementName, const std::string &caption);

 protected:
  void renderImpl() override;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUI_ELEMENTS_TREE_H
