//
// Created by xflajs00 on 16.11.2021.
//

#ifndef PF_IMGUI_DIALOGS_NOTIFICATION_H
#define PF_IMGUI_DIALOGS_NOTIFICATION_H

#include <chrono>
#include <pf_imgui/_export.h>
#include <pf_imgui/common/Font.h>
#include <pf_imgui/common/Label.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Renderable.h>

namespace pf::ui::ig {

enum class NotificationPhase { FadeIn, Wait, FadeOut, Expired };

class PF_IMGUI_EXPORT Notification : public Renderable, public ElementContainer {
  friend class NotificationManager;
  constexpr static inline std::chrono::milliseconds DEFAULT_DURATION{3000};
  constexpr static inline std::chrono::milliseconds FADE_IN_TIME{150};
  constexpr static inline std::chrono::milliseconds FADE_OUT_TIME{150};

  constexpr static inline float PADDING_X = 20.f;
  constexpr static inline float PADDING_Y = 20.f;
  constexpr static inline float PADDING_MESSAGE_Y = 10.f;

 public:
  Notification(std::string_view elementName, std::string_view labelText,
               std::chrono::milliseconds duration = DEFAULT_DURATION);

  void setIcon(const char *icon, Font newIconFont = Font::Default());
  void setIconColor(Color newColor);

  Property<Label> label;

 protected:
  void renderImpl() override;

 private:
  float height = 0.f;
  NotificationPhase currentPhase = NotificationPhase::FadeIn;

  const char *icon = nullptr;
  Font iconFont = Font::Default();
  std::optional<Color> iconColor = std::nullopt;

  std::chrono::steady_clock::time_point creationTime = std::chrono::steady_clock::now();
  std::chrono::milliseconds dismissDuration;

  bool firstRender = true;

  inline static const ImGuiWindowFlags flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav;

  void checkPhase();

  [[nodiscard]] float getOpacity() const;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_DIALOGS_NOTIFICATION_H
