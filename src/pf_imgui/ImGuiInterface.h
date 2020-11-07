//
// Created by petr on 10/27/20.
//

#ifndef PF_IMGUI_IMGUIINTERFACE_H
#define PF_IMGUI_IMGUIINTERFACE_H

#include "elements/MenuBars.h"
#include "elements/interface/Container.h"
#include "fwd.h"
#include <imgui.h>
#include <pf_imgui/_export.h>
#include <toml++/toml.h>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT ImGuiInterface : public Container {
 public:
  explicit ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig);

  [[nodiscard]] ImGuiIO &getIo() const;

  std::shared_ptr<Dialog> createDialog(const std::string &elementName,
                                       const std::string &caption, Modal modal = Modal::Yes);

  [[nodiscard]] AppMenuBar &getMenuBar();
  [[nodiscard]] bool hasMenuBar() const;

  [[nodiscard]] const toml::table &getConfig() const;

  void updateConfig();
  void setStateFromConfig();

  void setStyle(std::invocable<ImGuiStyle &> auto styleSetter) {
    styleSetter(ImGui::GetStyle());
  }

 protected:
  std::optional<AppMenuBar> menuBar = std::nullopt;

 private:
  static ImGuiIO &baseInit(ImGuiConfigFlags flags);
  ImGuiIO &io;

  toml::table config;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUIINTERFACE_H
