//
// Created by petr on 10/27/20.mvoe
//

#ifndef PF_IMGUI_IMGUIINTERFACE_H
#define PF_IMGUI_IMGUIINTERFACE_H

#include "elements/FileDialog.h"
#include "elements/MenuBars.h"
#include "elements/interface/Container.h"
#include "fwd.h"
#include <imgui.h>
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/_export.h>
#include <toml++/toml_table.hpp>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT ImGuiInterface : public Container {
 public:
  explicit ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig);

  [[nodiscard]] ImGuiIO &getIo() const;

  Dialog &createDialog(const std::string &elementName, const std::string &caption,
                       Modal modal = Modal::Yes);

  [[nodiscard]] AppMenuBar &getMenuBar();
  [[nodiscard]] bool hasMenuBar() const;

  [[nodiscard]] const toml::table &getConfig() const;

  [[nodiscard]] bool isWindowHovered() const;

  [[nodiscard]] bool isKeyboardCaptured() const;

  void updateConfig();
  void setStateFromConfig();

  void setStyle(std::invocable<ImGuiStyle &> auto styleSetter) { styleSetter(ImGui::GetStyle()); }

  void openFileDialog(const std::string &caption,
                      const std::vector<FileExtensionSettings> &extSettings,
                      std::invocable<std::vector<std::string>> auto onSelect,
                      std::invocable auto onCancel, std::string startPath = ".",
                      std::string startName = "", Modal modality = Modal::No,
                      uint32_t maxSelectedFiles = 1) {
    using namespace std::string_literals;
    fileDialogs.emplace_back("FileDialog"s + std::to_string(getNext(idGen)), caption, extSettings,
                             onSelect, onCancel, startPath, startName, modality, maxSelectedFiles);
  }

  void openDirDialog(const std::string &caption,
                     std::invocable<std::vector<std::string>> auto onSelect,
                     std::invocable auto onCancel, std::string startPath = ".",
                     std::string startName = "", Modal modality = Modal::No,
                     uint32_t maxSelectedFiles = 1) {
    using namespace std::string_literals;
    fileDialogs.emplace_back("FileDialog"s + std::to_string(getNext(idGen)), caption, onSelect,
                             onCancel, startPath, startName, modality, maxSelectedFiles);
  }

 protected:
  std::unique_ptr<AppMenuBar> menuBar = nullptr;

  void renderFileDialogs();

 private:
  static ImGuiIO &baseInit(ImGuiConfigFlags flags);
  ImGuiIO &io;
  std::vector<FileDialog> fileDialogs;
  cppcoro::generator<std::size_t> idGen = iota<std::size_t>();

  toml::table config;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUIINTERFACE_H
