//
// Created by xflajs00 on 16.11.2021.
//

#ifndef PF_IMGUI_NOTIFICATION_H
#define PF_IMGUI_NOTIFICATION_H

#include <chrono>
#include <pf_imgui/_export.h>
#include <pf_imgui/interface/Closeable.h>
#include <pf_imgui/interface/Customizable.h>
#include <pf_imgui/interface/ElementContainer.h>
#include <pf_imgui/interface/Labellable.h>
#include <pf_imgui/interface/Renderable.h>
#include <string>

namespace pf::ui::ig {

enum class NotificationPhase { FadeIn, Wait, FadeOut, Expired };

class PF_IMGUI_EXPORT Notification : public Renderable, public ElementContainer, public Labellable {
  friend class NotificationManager;
  constexpr static inline std::chrono::milliseconds DEFAULT_DURATION{3000};
  constexpr static inline std::chrono::milliseconds FADE_IN_TIME{150};
  constexpr static inline std::chrono::milliseconds FADE_OUT_TIME{150};

  constexpr static inline float PADDING_X = 20.f;
  constexpr static inline float PADDING_Y = 20.f;
  constexpr static inline float PADDING_MESSAGE_Y = 10.f;

 public:
  Notification(const std::string &name, const std::string &label,
               std::chrono::milliseconds duration = DEFAULT_DURATION);

  void setIcon(const char *icon, ImFont *font = nullptr);
  void setIconColor(Color newColor);

 protected:
  void renderImpl() override;

 private:
  float height = 0.f;
  NotificationPhase currentPhase = NotificationPhase::FadeIn;

  const char *icon = nullptr;
  ImFont *iconFont = nullptr;
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
#endif  // PF_IMGUI_NOTIFICATION_H
