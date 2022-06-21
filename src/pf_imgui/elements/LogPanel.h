/**
 * @file LogPanel.h
 * @brief ElementWithID to visualize log data.
 * @author Petr Flajšingr
 * @date 14.4.22
 */

#ifndef PF_IMGUI_ELEMENTS_LOGPANEL_H
#define PF_IMGUI_ELEMENTS_LOGPANEL_H

#include <array>
#include <imgui.h>
#include <pf_common/Explicit.h>
#include <pf_common/enums.h>
#include <pf_imgui/Color.h>
#include <pf_imgui/_export.h>
#include <pf_imgui/details/CustomIconButtonImpls.h>
#include <pf_imgui/interface/ElementWithID.h>
#include <pf_imgui/interface/Resizable.h>
#include <ringbuffer.hpp>
#include <sstream>

namespace pf::ui::ig {

/**
 *  EXAMPLE SPDLOG SINK
 *
 * @code{.cpp}
 * template<typename Mutex>
 * class PfImguiLogSink : public spdlog::sinks::base_sink<Mutex> {
 *  public:
 *   template<std::size_t LogLimit>
 *   explicit PfImguiLogSink(pf::ui::ig::LogPanel<spdlog::level::level_enum, LogLimit> &logPanel)
 *     : spdlog::sinks::base_sink<Mutex>(),
 *       addRecord([this, &logPanel](auto level, auto message) { logPanel.addRecord(level, message); }) {
 *     logPanel.addDestroyListener([this] { panelValid = false; });
 *   }
 *   template<std::size_t LogLimit>
 *   PfImguiLogSink(pf::ui::ig::LogPanel<spdlog::level::level_enum, LogLimit> &logPanel,
 *                const std::unique_ptr<spdlog::formatter> &formatter)
 *     : spdlog::sinks::base_sink<Mutex>(formatter),
 *       addRecord([this, &logPanel](auto level, auto message) { logPanel.addRecord(level, message); }) {
 *     logPanel.addDestroyListener([this] { panelValid = false; });
 *   }
 *
 *   protected:
 *   void sink_it_(const spdlog::details::log_msg &msg) override {
 *     if (!panelValid) { return; }
 *     spdlog::memory_buf_t formatted;
 *     spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
 *     addRecord(msg.level, std::string_view{formatted.begin(), formatted.end()});
 *   }
 *   void flush_() override {}
 *
 *  private:
 *   bool panelValid = true;
 *   std::function<void(spdlog::level::level_enum, std::string_view)> addRecord;
 * };
 * using PfImguiLogSink_mt = PfImguiLogSink<std::mutex>;
 * using PfImguiLogSink_st = PfImguiLogSink<spdlog::details::null_mutex>;
 * @endcode
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
class PF_IMGUI_EXPORT LogPanel : public ElementWithID, public Resizable, public Savable {
  struct Record {
    Category category;
    std::string text;
    Color color;
    std::optional<Color> backgroundColor = std::nullopt;
    bool show = true;
  };

 public:
  /**
   * @brief Construction args
   */
  struct Config {
    using Parent = LogPanel;
    Explicit<std::string_view> name; /*!< Unique name of the element */
    Size size = Size::Auto();        /*!< Size of the element */
    bool persistent = false;         /*!< Enable toggle buttons state saving */
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
   * @param persistent enable toggle buttons state saving
   */
  LogPanel(const std::string &name, Size size, Persistent persistent = Persistent::No);
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
   * Set a new text color for records in given category.
   * @param category category to modify
   * @param color new color of text
   */
  void setCategoryTextColor(Category category, Color color);
  /**
   * Set a new background color for records in given category.
   * @param category category to modify
   * @param color new color of background
   */
  void setCategoryBackgroundColor(Category category, Color color);
  /**
   * Enable/disable category for record filtering.
   * @param category category to enable/disable
   * @param enabled new category state
   */
  void setCategoryEnabled(Category category, bool enabled);
  /**
   * Enable/disable category for being shown in the UI.
   * @param category category to enable/disable
   * @param catAllowed new category state
   */
  void setCategoryAllowed(Category category, bool catAllowed);

  [[nodiscard]] toml::table toToml() const override;
  void setFromToml(const toml::table &src) override;

  FullColorPalette color;
  FullStyleOptions style;
  Font font = Font::Default();

 protected:
  void renderImpl() override;

 private:
  void renderTextArea();

  [[nodiscard]] bool renderCategoryCombobox();

  bool isAllowedRecord(const Record &record);
  void refreshAllowedRecords();
  bool isAllowedCategory(Category category);
  bool isAllowedText(const std::string &text);

  static void DrawTextBackground(const char *str, Color color, bool textWrapped, bool applyPadding);

  constexpr static std::size_t GetCategoryIndex(Category category);

  constexpr static std::size_t BUFFER_SIZE = 500;
  jnk0le::Ringbuffer<Record, RecordLimit> records;

  std::array<Color, magic_enum::enum_count<Category>()> categoryTextColors;
  std::array<std::optional<Color>, magic_enum::enum_count<Category>()> categoryBackgroundColors;
  std::array<std::string, magic_enum::enum_count<Category>()> categoryStrings;

  std::array<bool, magic_enum::enum_count<Category>()> categoryAllowed;

  std::array<bool, magic_enum::enum_count<Category>()> categoryEnabled;
  char filterBuffer[BUFFER_SIZE] = "\0";
  std::size_t filterStringSize = 0;

  WrapTextToggle wrapTextToggle;
  ScrollToEndToggle scrollToEndToggle;
  CopyToClipboardButton copyToClipboardButton;
  TrashcanButton clearButton;
};

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
LogPanel<Category, RecordLimit>::LogPanel(Config &&config)
    : LogPanel(std::string{config.name.value}, config.size, config.persistent ? Persistent::Yes : Persistent::No) {}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
LogPanel<Category, RecordLimit>::LogPanel(const std::string &name, Size size, Persistent persistent)
    : ElementWithID(name), Resizable(size), Savable(persistent), wrapTextToggle("wrapText"),
      scrollToEndToggle("scrollToEnd"), copyToClipboardButton("copyToClipboard"), clearButton("clear") {
  std::size_t i = 0;
  for (const auto category : magic_enum::enum_values<Category>()) {
    categoryStrings[i++] = magic_enum::enum_name(category);
  }
  std::ranges::fill(categoryEnabled, true);
  std::ranges::fill(categoryAllowed, true);
  std::ranges::fill(categoryTextColors, Color::White);
  std::ranges::fill(categoryBackgroundColors, std::nullopt);

  clearButton.addClickListener([this] { records.remove(records.readAvailable()); });
  copyToClipboardButton.addClickListener([this] { ImGui::SetClipboardText(getText().c_str()); });
  clearButton.setTooltip("Clear log");
  copyToClipboardButton.setTooltip("Copy log to clipboard");
  wrapTextToggle.setTooltip("Wrap text");
  scrollToEndToggle.setTooltip("Autoscroll down");
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::addRecord(Category category, std::string_view text) {
  if (records.writeAvailable() == 0) { records.remove(); }
  const auto categoryIndex = GetCategoryIndex(category);
  auto newRecord =
      Record{category, std::string{text}, categoryTextColors[categoryIndex], categoryBackgroundColors[categoryIndex]};
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
void LogPanel<Category, RecordLimit>::setCategoryTextColor(Category category, Color color) {
  categoryTextColors[GetCategoryIndex(category)] = color;
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::setCategoryBackgroundColor(Category category, Color color) {
  categoryBackgroundColors[GetCategoryIndex(category)] = color;
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::setCategoryEnabled(Category category, bool enabled) {
  categoryEnabled[GetCategoryIndex(category)] = enabled;
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::setCategoryAllowed(Category category, bool catAllowed) {
  categoryAllowed[GetCategoryIndex(category)] = catAllowed;
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  auto filterChanged = false;
  ImGui::BeginHorizontal("layout", static_cast<ImVec2>(getSize()), 0);
  {
    ImGui::BeginChild("controls_area", ImVec2{25, 0});
    {
      ImGui::BeginVertical("controls", ImVec2{0, 0}, 0);
      {
        wrapTextToggle.render();
        ImGui::Spring(0);
        scrollToEndToggle.render();
        ImGui::Spring(0);
        copyToClipboardButton.render();
        ImGui::Spring(0);
        clearButton.render();
      }
      ImGui::EndVertical();
    }
    ImGui::EndChild();
    ImGui::Spring(0.f);
    ImGui::BeginVertical("log", ImVec2{0, 0}, 0);
    {
      ImGui::BeginHorizontal("log_filter", ImVec2{0, 25}, 0);
      {
        filterChanged |= renderCategoryCombobox();
        ImGui::Spring(0.f);
        ImGui::SetNextItemWidth(-50.f);
        if (ImGui::InputText("Filter", filterBuffer, BUFFER_SIZE)) {
          filterChanged = true;
          if (filterBuffer[0] == '\0') {
            filterStringSize = 0;
          } else {
            filterStringSize = std::strlen(filterBuffer);
          }
        }
      }
      ImGui::EndHorizontal();
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
  const auto wrapEnabled = wrapTextToggle.getValue();
  if (ImGui::BeginChild(getName().c_str(), ImVec2{0, 0}, true,
                        wrapTextToggle.getValue() ? ImGuiWindowFlags{} : ImGuiWindowFlags_HorizontalScrollbar)) {
    for (std::size_t i = 0; i < records.readAvailable(); ++i) {
      const auto &record = records[i];
      if (!record.show) { continue; }
      ImGui::PushStyleColor(ImGuiCol_Text, record.color);
      if (record.backgroundColor.has_value()) {
        DrawTextBackground(record.text.c_str(), *record.backgroundColor, wrapEnabled, false);
      }
      if (wrapEnabled) {
        ImGui::TextWrapped(record.text.c_str());
      } else {
        ImGui::Text(record.text.c_str());
      }
      ImGui::PopStyleColor(1);
    }
    if (scrollToEndToggle.getValue()) { ImGui::SetScrollHereY(1.0f); }
  }
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0) bool
LogPanel<Category, RecordLimit>::renderCategoryCombobox() {
  bool filterChanged = false;
  std::size_t i = 0;
  ImGui::SetNextItemWidth(120);
  if (ImGui::BeginCombo("##categories", "Categories")) {
    std::ranges::for_each(categoryEnabled, [&](bool &catEnabled) {
      if (!categoryAllowed[i]) { return; }
      if (categoryBackgroundColors[i].has_value()) {
        DrawTextBackground(categoryStrings[i].c_str(), categoryBackgroundColors[i].value(), false, true);
      }
      ImGui::PushStyleColor(ImGuiCol_Text, categoryTextColors[i]);
      const auto categoryChanged = ImGui::Checkbox(categoryStrings[i++].c_str(), &catEnabled);
      filterChanged = filterChanged || categoryChanged;
      ImGui::PopStyleColor(1);
    });
    ImGui::EndCombo();
  }
  if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Filter categories"); }
  return filterChanged;
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

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::DrawTextBackground(const char *str, Color color, bool textWrapped,
                                                         bool applyPadding) {
  const auto maxWidth = ImGui::GetContentRegionAvail().x;
  const auto textSize = ImGui::CalcTextSize(str, nullptr, false, textWrapped ? maxWidth : -1.f);
  ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetCursorScreenPos(),
                                            ImGui::GetCursorScreenPos() + ImVec2{maxWidth, textSize.y}
                                                + (applyPadding ? ImGui::GetStyle().FramePadding * 2 : ImVec2{0, 0}),
                                            static_cast<ImU32>(color));
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
constexpr std::size_t LogPanel<Category, RecordLimit>::GetCategoryIndex(Category category) {
  return *magic_enum::enum_index(category);
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
toml::table LogPanel<Category, RecordLimit>::toToml() const {
  return toml::table{{"textwrap", wrapTextToggle.getValue()}, {"autoscroll", scrollToEndToggle.getValue()}};
}

template<Enum Category, std::size_t RecordLimit>
  requires((RecordLimit & (RecordLimit - 1)) == 0)
void LogPanel<Category, RecordLimit>::setFromToml(const toml::table &src) {
  if (const auto iter = src.find("textwrap"); iter != src.end()) {
    if (const auto ptr = iter->second.as_boolean(); ptr != nullptr) { wrapTextToggle.setValue(ptr->get()); }
  }
  if (const auto iter = src.find("autoscroll"); iter != src.end()) {
    if (const auto ptr = iter->second.as_boolean(); ptr != nullptr) { scrollToEndToggle.setValue(ptr->get()); }
  }
}

}  // namespace pf::ui::ig

#endif  //PF_IMGUI_ELEMENTS_LOGPANEL_H
