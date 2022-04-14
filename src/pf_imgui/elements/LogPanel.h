/**
 * @file LogPanel.h
 * @brief Element to visualize log data.
 * @author Petr Flajšingr
 * @date 14.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_LOGPANEL_H
#define PF_IMGUI_ELEMENTS_LOGPANEL_H

#include <array>
#include <imgui.h>
#include <pf_common/enums.h>
#include <pf_imgui/Color.h>
#include <pf_imgui/interface/Element.h>
#include <pf_imgui/interface/Resizable.h>
#include <ringbuffer.hpp>
#include <sstream>
#include <pf_imgui/_export.h>

namespace pf::ui::ig {

/**
    EXAMPLE SPDLOG SINK

```
template<typename Mutex>
class PfImguiLogSink : public spdlog::sinks::base_sink<Mutex> {
 public:
  template<std::size_t LogLimit>
  explicit PfImguiLogSink(pf::ui::ig::LogPanel<spdlog::level::level_enum, LogLimit> &logPanel)
    : spdlog::sinks::base_sink<Mutex>(),
      addRecord([this, &logPanel](auto level, auto message) { logPanel.addRecord(level, message); }) {
    logPanel.addDestroyListener([this] { panelValid = false; });
  }
  template<std::size_t LogLimit>
  PfImguiLogSink(pf::ui::ig::LogPanel<spdlog::level::level_enum, LogLimit> &logPanel,
               const std::unique_ptr<spdlog::formatter> &formatter)
    : spdlog::sinks::base_sink<Mutex>(formatter),
      addRecord([this, &logPanel](auto level, auto message) { logPanel.addRecord(level, message); }) {
    logPanel.addDestroyListener([this] { panelValid = false; });
  }

  protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {
    if (!panelValid) { return; }
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    addRecord(msg.level, std::string_view{formatted.begin(), formatted.end()});
  }
  void flush_() override {}

 private:
  bool panelValid = true;
  std::function<void(spdlog::level::level_enum, std::string_view)> addRecord;
};
using PfImguiLogSink_mt = PfImguiLogSink<std::mutex>;
using PfImguiLogSink_st = PfImguiLogSink<spdlog::details::null_mutex>;
```
 */

/**
 * @brief An element displaying log data.
 *
 * It can color records based on category. There is a text input for substring filtering and checkboxes for categories.
 *
 * @todo: better text filtering
 *
 * @tparam Category Log categories - checkboxes are generated to enable/disable them
 * @tparam RecordLimit limit of records held in the element
 */
template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)  // RecordLimit has to be power of two
class PF_IMGUI_EXPORT LogPanel : public Element, public Resizable {
  struct Record {
    Category category;
    std::string text;
    Color color;
    bool show = true;
  };

 public:
  /**
   * @brief Construction args
   */
  struct Config {
    using Parent = LogPanel;
    std::string_view name;    /*!< Unique name of the element */
    Size size = Size::Auto(); /*!< Size of the element */
  };
  /**
   * Construct LogPanel
   * @param config construction args @see LogPanel::Config
   */
  explicit LogPanel(Config &&config);
  /**
   * Construct LogPanel
   * @param name unique name of the element
   * @param size size of the element
   */
  LogPanel(const std::string &name, Size size);
  /**
   * Add a record to the output,
   * @param category logging category
   * @param text text to render
   */
  void addRecord(Category category, std::string_view text);
  /**
   * @return all text stored inside the element currently
   */
  [[nodiscard]] std::string getText() const;
  /**
   * Set a new color for records in given category.
   * @param category category to modify
   * @param color new color of the category
   */
  void setCategoryColor(Category category, Color color);
  /**
   * Enable/disable category for both records and controls.
   * @param category category to enable/disable
   * @param enabled new category state
   */
  void setCategoryEnabled(Category category, bool enabled);

 protected:
  void renderImpl() override;

 private:
  void renderTextArea();

  bool isAllowedRecord(const Record &record);
  void refreshAllowedRecords();
  bool isAllowedCategory(Category category);
  bool isAllowedText(const std::string &text);

  constexpr static std::size_t BUFFER_SIZE = 500;
  jnk0le::Ringbuffer<Record, RecordLimit> records;

  std::array<Color, magic_enum::enum_count<Category>()> categoryColors;
  std::array<std::string, magic_enum::enum_count<Category>()> categoryStrings;

  std::array<bool, magic_enum::enum_count<Category>()> categoryAllowed;

  bool scrollToBottom = false;
  bool textWrapEnabled = true;
  std::array<bool, magic_enum::enum_count<Category>()> categoryEnabled;
  char filterBuffer[BUFFER_SIZE] = "\0";
  std::size_t filterStringSize = 0;
};

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
LogPanel<Category, RecordLimit>::LogPanel(Config &&config) : LogPanel(std::string{config.name}, config.size) {}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
LogPanel<Category, RecordLimit>::LogPanel(const std::string &name, Size size) : Element(name), Resizable(size) {
  std::size_t i = 0;
  for (const auto category : magic_enum::enum_values<Category>()) {
    categoryStrings[i++] = magic_enum::enum_name(category);
  }
  std::ranges::fill(categoryEnabled, true);
  std::ranges::fill(categoryAllowed, true);
  std::ranges::fill(categoryColors, Color::White);
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::addRecord(Category category, std::string_view text) {
  if (records.writeAvailable() == 0) { records.remove(); }
  auto newRecord = Record{category, std::string{text}, categoryColors[*magic_enum::enum_index(category)]};
  newRecord.show = isAllowedRecord(newRecord);
  records.insert(newRecord);
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
std::string LogPanel<Category, RecordLimit>::getText() const {
  std::stringstream ss{};
  for (std::size_t i = 0; i < records.readAvailable(); ++i) {
    const auto &record = records[i];
    ss << record.text << '\n';
  }
  return ss.str();
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::setCategoryColor(Category category, Color color) {
  categoryColors[*magic_enum::enum_index(category)] = color;
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::setCategoryEnabled(Category category, bool enabled) {
  categoryAllowed[*magic_enum::enum_index(category)] = enabled;
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::renderImpl() {
  auto filterChanged = false;
  ImGui::BeginHorizontal("layout", static_cast<ImVec2>(getSize()), 0);
  {
    ImGui::BeginChild("controls_area", ImVec2{100, 0});
    {
      ImGui::BeginVertical("controls", ImVec2{0, 0}, 0);
      {
        ImGui::BeginHorizontal("buttons", ImVec2{0, 20}, 0);
        {
          if (ImGui::Button("Clear")) { records.remove(records.readAvailable()); }
          ImGui::Spring(0.f);
          if (ImGui::Button("Copy")) { ImGui::SetClipboardText(getText().c_str()); }
        }
        ImGui::EndHorizontal();
        ImGui::Spring(0.f);
        ImGui::Separator();
        ImGui::Spring(0.f);
        ImGui::Checkbox("Autoscroll", &scrollToBottom);
        ImGui::Spring(0.f);
        ImGui::Checkbox("Wrap text", &textWrapEnabled);
        ImGui::Spring(0.f);
        ImGui::Separator();
        ImGui::Spring(0.f);
        std::size_t i = 0;
        std::ranges::for_each(categoryEnabled, [&](bool &enabled) {
          if (!categoryAllowed[i]) { return; }
          ImGui::PushStyleColor(ImGuiCol_Text, categoryColors[i]);
          filterChanged = filterChanged | ImGui::Checkbox(categoryStrings[i++].c_str(), &enabled);
          ImGui::Spring(0.f);
          ImGui::PopStyleColor(1);
        });
      }
      ImGui::EndVertical();
    }
    ImGui::EndChild();
    ImGui::Spring(0.f);
    ImGui::BeginVertical("log", ImVec2{0, 0}, 0);
    {
      ImGui::SetNextItemWidth(-50.f);
      if (ImGui::InputText("Filter", filterBuffer, BUFFER_SIZE)) {
        filterChanged = true;
        if (filterBuffer[0] == '\0') {
          filterStringSize = 0;
        } else {
          filterStringSize = std::strlen(filterBuffer);
        }
      }
      ImGui::Spring(0.f);
      if (filterChanged) { refreshAllowedRecords(); }
      renderTextArea();
    }
    ImGui::EndVertical();
  }
  ImGui::EndHorizontal();
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::renderTextArea() {
  RAII end{ImGui::EndChild};
  if (ImGui::BeginChild(getName().c_str(), ImVec2{0, 0}, true,
                        textWrapEnabled ? ImGuiWindowFlags{} : ImGuiWindowFlags_HorizontalScrollbar)) {
    for (std::size_t i = 0; i < records.readAvailable(); ++i) {
      const auto &record = records[i];
      if (!record.show) { continue; }
      ImGui::PushStyleColor(ImGuiCol_Text, record.color);
      if (textWrapEnabled) {
        ImGui::TextWrapped(record.text.c_str());
      } else {
        ImGui::Text(record.text.c_str());
      }
      ImGui::PopStyleColor(1);
    }
    if (scrollToBottom) { ImGui::SetScrollHereY(1.0f); }
  }
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0) bool
LogPanel<Category, RecordLimit>::isAllowedRecord(const LogPanel::Record &record) {
  return isAllowedCategory(record.category) && isAllowedText(record.text);
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::refreshAllowedRecords() {
  for (std::size_t i = 0; i < records.readAvailable(); ++i) { records[i].show = isAllowedRecord(records[i]); }
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0) bool
LogPanel<Category, RecordLimit>::isAllowedCategory(Category category) {
  const auto catIndex = *magic_enum::enum_index(category);
  return categoryEnabled[catIndex] && categoryAllowed[catIndex];
}
template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0) bool
LogPanel<Category, RecordLimit>::isAllowedText(const std::string &text) {
  if (filterStringSize == 0) { return true; }
  return text.find(std::string_view{filterBuffer, filterStringSize}) != std::string::npos;
}

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_LOGPANEL_H
