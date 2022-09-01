//
// Created by xflajs00 on 16.04.2022.
//

#include "NodeEditor.h"
#include <pf_imgui/node_editor/blueprint/common.h>
#include <pf_imgui/node_editor/blueprint/nodes/Node.h>

#include <pf_imgui/node_editor/blueprint/pins/ArrayPin.h>
#include <pf_imgui/node_editor/blueprint/pins/CheckboxPin.h>
#include <pf_imgui/node_editor/blueprint/pins/DragPin.h>
#include <pf_imgui/node_editor/blueprint/pins/EventPin.h>
#include <pf_imgui/node_editor/blueprint/pins/ExecPin.h>
#include <pf_imgui/node_editor/blueprint/pins/StringInputPin.h>
#include <pf_imgui/node_editor/blueprint/pins/ValuePin.h>

#include <pf_imgui/node_editor/blueprint/nodes/NodeWithLabel.h>

namespace pf::ui::ig::bp {

NodeEditor::NodeEditor(const std::string &elementName, const Size &initialSize)
    : ig::NodeEditor(elementName, initialSize) {
  NodeEditorLoading::Get()->registerConstruction<ArrayPin<float>>();
  NodeEditorLoading::Get()->registerConstruction<ArrayPin<int>>();
  NodeEditorLoading::Get()->registerConstruction<CheckboxPin>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<float>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<glm::vec2>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<glm::vec3>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<glm::vec4>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<int>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<glm::ivec2>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<glm::ivec3>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<glm::ivec4>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<math::Range<int>>>();
  NodeEditorLoading::Get()->registerConstruction<DragPin<math::Range<float>>>();
  NodeEditorLoading::Get()->registerConstruction<EventPin>();
  NodeEditorLoading::Get()->registerConstruction<ExecPin>();
  NodeEditorLoading::Get()->registerConstruction<StringInputPin>();
  NodeEditorLoading::Get()->registerConstruction<ValuePin<int>>();
  NodeEditorLoading::Get()->registerConstruction<ValuePin<float>>();

  NodeEditorLoading::Get()->registerConstruction<NodeWithLabel>();
}

NodeEditor::NodeEditor(Config &&config) : NodeEditor(std::string{config.name.value}, config.size) {}

toml::table NodeEditor::toToml() const {
  auto nodesToml = toml::array{};
  std::ranges::for_each(getNodes(), [&](const ig::Node &node) {
    if (auto bpNode = dynamic_cast<const Node *>(&node); bpNode != nullptr) { nodesToml.push_back(bpNode->toToml()); }
  });
  const auto linksToml = linksToToml();
  const auto commentsToml = commentsToToml();
  return toml::table{{"nodes", nodesToml}, {"links", linksToml}, {"comments", commentsToml}};
}

void NodeEditor::setFromToml(const toml::table &src) {
  const auto nodeFromToml = [&](const toml::table &nodeSrc) -> std::optional<std::unique_ptr<Node>> {
    if (auto typeIter = nodeSrc.find("type"); typeIter != nodeSrc.end()) {
      if (auto typeValue = typeIter->second.as_string(); typeValue != nullptr) {
        return NodeEditorLoading::Get()->constructNode(typeValue->get(), this, src);
      }
    }
    return std::nullopt;
  };
  if (auto nodesIter = src.find("nodes"); nodesIter != src.end()) {
    if (auto nodesToml = nodesIter->second.as_array(); nodesToml != nullptr) {
      std::ranges::for_each(*nodesToml, [&](const auto &item) {
        if (auto nodeToml = item.as_table(); nodeToml != nullptr) {
          if (auto newNode = nodeFromToml(*nodeToml); newNode.has_value()) {
            (*newNode)->id = getNextId();
            nodes.emplace_back(std::move(*newNode));
          }
        }
      });
    }
  }
  if (auto commentsIter = src.find("comments"); commentsIter != src.end()) {
    if (auto commentsToml = commentsIter->second.as_array(); commentsToml != nullptr) {
      std::ranges::for_each(*commentsToml, [&](const auto &item) {
        if (auto commentToml = item.as_table(); commentToml != nullptr) {
          std::optional<Width> commentWidth = std::nullopt;
          std::optional<Height> commentHeight = std::nullopt;
          std::optional<std::string> commentLabel = std::nullopt;
          std::optional<std::string> commentName = std::nullopt;
          std::optional<Position> position = std::nullopt;
          if (auto iter = commentToml->find("name"); iter != commentToml->end()) {
            if (auto nameToml = iter->second.as_string(); nameToml != nullptr) { commentName = nameToml->get(); }
          }
          if (auto positionXIter = commentToml->find("positionX"); positionXIter != commentToml->end()) {
            if (auto positionXToml = positionXIter->second.as_floating_point(); positionXToml != nullptr) {
              if (auto positionYIter = commentToml->find("positionY"); positionYIter != commentToml->end()) {
                if (auto positionYToml = positionYIter->second.as_floating_point(); positionYToml != nullptr) {
                  position =
                      Position{static_cast<float>(positionXToml->get()), static_cast<float>(positionYToml->get())};
                }
              }
            }
          }
          if (auto iter = commentToml->find("label"); iter != commentToml->end()) {
            if (auto labelToml = iter->second.as_string(); labelToml != nullptr) { commentLabel = labelToml->get(); }
          }
          if (auto iter = commentToml->find("width"); iter != commentToml->end()) {
            if (auto widthToml = iter->second.as_floating_point(); widthToml != nullptr) {
              commentWidth = static_cast<float>(widthToml->get());
            }
          }
          if (auto iter = commentToml->find("height"); iter != commentToml->end()) {
            if (auto heightToml = iter->second.as_floating_point(); heightToml != nullptr) {
              commentHeight = static_cast<float>(heightToml->get());
            }
          }
          if (!commentWidth.has_value() || !commentHeight.has_value() || !commentLabel.has_value() || !commentName.has_value()) { return; }
          auto &newComment = addComment(*commentName, *commentLabel, Size{*commentWidth, *commentHeight});
          if (position.has_value()) { newComment.setPosition(*position); }
        }
      });
    }
  }
  if (auto linksIter = src.find("links"); linksIter != src.end()) {
    if (auto linksToml = linksIter->second.as_array(); linksToml != nullptr) {
      std::ranges::for_each(*linksToml, [&](const auto &item) {
        if (auto linkToml = item.as_table(); linkToml != nullptr) {
          std::optional<std::string> linkName = std::nullopt;
          std::optional<Pin *> inputPin = std::nullopt;
          std::optional<Pin *> outputPin = std::nullopt;
          std::optional<Color> color = std::nullopt;
          std::optional<float> thickness = std::nullopt;
          if (auto iter = linkToml->find("name"); iter != linkToml->end()) {
            if (auto nameToml = iter->second.as_string(); nameToml != nullptr) { linkName = nameToml->get(); }
          }
          if (auto iter = linkToml->find("inputPin"); iter != linkToml->end()) {
            if (auto inputPinNameToml = iter->second.as_string(); inputPinNameToml != nullptr) {
              auto pin = findPinByName(inputPinNameToml->get());
              if (pin.has_value()) { inputPin = static_cast<Pin *>(*pin); }
            }
          }
          if (auto iter = linkToml->find("outputPin"); iter != linkToml->end()) {
            if (auto outputPinNameToml = iter->second.as_string(); outputPinNameToml != nullptr) {
              auto pin = findPinByName(outputPinNameToml->get());
              if (pin.has_value()) { outputPin = static_cast<Pin *>(*pin); }
            }
          }
          if (auto iter = linkToml->find("color"); iter != linkToml->end()) {
            if (auto colorToml = iter->second.as_integer(); colorToml != nullptr) {
              color = Color{static_cast<ImU32>(colorToml->get())};
            }
          }
          if (auto iter = linkToml->find("thickness"); iter != linkToml->end()) {
            if (auto thicknessToml = iter->second.as_floating_point(); thicknessToml != nullptr) {
              thickness = static_cast<float>(thicknessToml->get());
            }
          }
          if (!linkName.has_value() || !inputPin.has_value() || !outputPin.has_value() || !color.has_value()
              || !thickness.has_value()) {
            return;
          }
          auto &newLink = addLink(*linkName, **inputPin, **outputPin);
          newLink.setColor(*color);
          newLink.setThickness(*thickness);
          newLink.setId(getNextId());
        }
      });
    }
  }
}

toml::array NodeEditor::linksToToml() const {
  auto result = toml::array{};
  std::ranges::for_each(getLinks(), [&](const Link &link) {
    if (!link.isValid()) { return; }
    result.push_back(toml::table{
        {"name", link.getName()},
        {"inputPin", link.getInputPin().getName()},
        {"outputPin", link.getOutputPin().getName()},
        {"color", static_cast<ImU32>(link.getColor())},
        {"thickness", link.getThickness()},
    });
  });
  return result;
}

toml::array NodeEditor::commentsToToml() const {
  auto result = toml::array();
  std::ranges::for_each(comments, [&](const auto &comment) {
    result.push_back(toml::table{{"name", comment->getName()},
                                 {"label", comment->label.get()},
                                 {"width", static_cast<float>(comment->getSize().width)},
                                 {"height", static_cast<float>(comment->getSize().height)},
                                 {"positionX", comment->getPosition().x},
                                 {"positionY", comment->getPosition().y}});
  });
  return result;
}

}  // namespace pf::ui::ig::bp
