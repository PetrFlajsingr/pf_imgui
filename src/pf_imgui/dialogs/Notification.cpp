//
// Created by xflajs00 on 16.11.2021.
//

#include "Notification.h"

namespace pf::ui::ig {

Notification::Notification(const std::string &name, const std::string &label, std::chrono::milliseconds duration)
    : Renderable(name), Labellable(label), dismissDuration(duration) {}

void Notification::renderImpl() {
  if (firstRender) {
    firstRender = false;
    ImGui::SetNextWindowFocus();
    if (!iconColor.has_value()) { iconColor = ImGui::GetStyleColorVec4(ImGuiCol_Text); }
  }
  checkPhase();
  const auto vp_size = ImGui::GetMainViewport()->Size;
  ImGui::SetNextWindowPos(ImVec2(vp_size.x - PADDING_X, vp_size.y - PADDING_Y - height), ImGuiCond_Always,
                          ImVec2(1.0f, 1.0f));

  const auto opacity = getOpacity();
  ImGui::SetNextWindowBgAlpha(opacity);
  RAII end{[] { ImGui::End(); }};
  if (ImGui::Begin(fmt::format("##{}", getName()).c_str(), nullptr, flags)) {
    ImGui::PushTextWrapPos(vp_size.x / 3.f);
    RAII endTextWrap{[] { ImGui::PopTextWrapPos(); }};

    if (icon != nullptr) {
      ImGui::PushFont(iconFont);
      RAII endFont{[] { ImGui::PopFont(); }};
      ImGui::TextColored(*iconColor, icon);
      ImGui::SameLine();
    }
    ImGui::Text(getLabel().c_str());
    /*if (was_title_rendered && !NOTIFY_NULL_OR_EMPTY(content)) {
        SetCursorPosY(GetCursorPosY() + 5.f);// Must be a better way to do this!!!!
      }*/
    ImGui::Separator();
    std::ranges::for_each(getChildren(), [](auto &child) { child.render(); });
  }
  height += ImGui::GetWindowHeight() + PADDING_MESSAGE_Y;
}

void Notification::setIcon(const char *newIcon, ImFont *font) {
  Notification::icon = newIcon;
  Notification::iconFont = font;
}

void Notification::setIconColor(const ImVec4 &newColor) { Notification::iconColor = newColor; }

void Notification::checkPhase() {
  const auto timeElapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creationTime);
  if (timeElapsed > FADE_IN_TIME + FADE_OUT_TIME + dismissDuration) {
    currentPhase = NotificationPhase::Expired;
  } else if (timeElapsed > FADE_IN_TIME + dismissDuration) {
    currentPhase = NotificationPhase::FadeOut;
  } else if (timeElapsed > FADE_IN_TIME) {
    currentPhase = NotificationPhase::Wait;
  } else {
    currentPhase = NotificationPhase::FadeIn;
  }
}

float Notification::getOpacity() const {
  const auto timeElapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creationTime);
  switch (currentPhase) {
    case NotificationPhase::FadeIn:
      return (static_cast<float>(timeElapsed.count()) / static_cast<float>(FADE_IN_TIME.count()));
    case NotificationPhase::FadeOut:
      return (1.f
              - ((static_cast<float>(timeElapsed.count()) - static_cast<float>(FADE_IN_TIME.count())
                  - static_cast<float>(dismissDuration.count()))
                 / static_cast<float>(FADE_OUT_TIME.count())));
    default: return 1.f;
  }
}

}// namespace pf::ui::ig