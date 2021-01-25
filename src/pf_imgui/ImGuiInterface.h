//
// Created by petr on 10/27/20.mvoe
//

#ifndef PF_IMGUI_IMGUIINTERFACE_H
#define PF_IMGUI_IMGUIINTERFACE_H

#include "elements/FileDialog.h"
#include "elements/MenuBars.h"
#include "elements/Window.h"
#include "elements/interface/Container.h"
#include "fwd.h"
#include <imgui.h>
#include <memory>
#include <pf_common/coroutines/Sequence.h>
#include <pf_imgui/_export.h>
#include <string>
#include <toml++/toml.h>
#include <vector>

namespace pf::ui::ig {

class PF_IMGUI_EXPORT ImGuiInterface : public Renderable {
 public:
  explicit ImGuiInterface(ImGuiConfigFlags flags, toml::table tomlConfig);

  [[nodiscard]] ImGuiIO &getIo() const;

  Dialog &createDialog(const std::string &elementName, const std::string &caption, Modal modal = Modal::Yes);

  Window &createWindow(const std::string &windowName, std::string title);
  void removeWindow(const std::string &name);

  Window &windowByName(const std::string &name);

  inline auto getWindows() {
    return windows | ranges::views::transform([](auto &window) -> Window & { return *window; });
  }

  [[nodiscard]] AppMenuBar &getMenuBar();
  [[nodiscard]] bool hasMenuBar() const;

  [[nodiscard]] const toml::table &getConfig() const;

  [[nodiscard]] bool isWindowHovered() const;

  [[nodiscard]] bool isKeyboardCaptured() const;


  void updateConfig();
  void setStateFromConfig();

  void setStyle(std::invocable<ImGuiStyle &> auto styleSetter) { styleSetter(ImGui::GetStyle()); }

  void openFileDialog(const std::string &caption, const std::vector<FileExtensionSettings> &extSettings,
                      std::invocable<std::vector<std::string>> auto onSelect, std::invocable auto onCancel,
                      std::string startPath = ".", std::string startName = "", Modal modality = Modal::No,
                      uint32_t maxSelectedFiles = 1) {
    using namespace std::string_literals;
    fileDialogs.emplace_back("FileDialog"s + std::to_string(getNext(idGen)), caption, extSettings, onSelect, onCancel,
                             startPath, startName, modality, maxSelectedFiles);
  }

  void openDirDialog(const std::string &caption, std::invocable<std::vector<std::string>> auto onSelect,
                     std::invocable auto onCancel, std::string startPath = ".", std::string startName = "",
                     Modal modality = Modal::No, uint32_t maxSelectedFiles = 1) {
    using namespace std::string_literals;
    fileDialogs.emplace_back("FileDialog"s + std::to_string(getNext(idGen)), caption, onSelect, onCancel, startPath,
                             startName, modality, maxSelectedFiles);
  }

 protected:
  std::unique_ptr<AppMenuBar> menuBar = nullptr;

  void renderFileDialogs();

  void renderImpl() override;

 private:
  struct DialogContainer : public Container {};

  DialogContainer dialogContainer;

  static ImGuiIO &baseInit(ImGuiConfigFlags flags);
  ImGuiIO &io;
  std::vector<FileDialog> fileDialogs;
  cppcoro::generator<std::size_t> idGen = iota<std::size_t>();

  std::vector<std::unique_ptr<Window>> windows;

  toml::table config;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_IMGUIINTERFACE_H
