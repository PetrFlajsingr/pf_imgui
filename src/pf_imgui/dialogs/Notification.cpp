//
// Created by xflajs00 on 16.11.2021.
//

#include "Notification.h"
#include <fmt/format.h>
#include <imgui_internal.h>

namespace pf::ui::ig {

Notification::Notification(std::string_view elementName, std::string_view labelText, std::chrono::milliseconds duration)
    : Renderable(elementName), label(std::string{labelText}), dismissDuration(duration) {}

void Notification::renderImpl() {
  [[maybe_unused]] auto colorScoped = color.applyScoped();
  [[maybe_unused]] auto styleScoped = style.applyScoped();
  [[maybe_unused]] auto fontScoped = font.applyScopedIfNotDefault();
  if (firstRender) {
    firstRender = false;
    ImGui::SetNextWindowFocus();
    if (!iconColor.has_value()) { iconColor = Color{ImGui::GetStyleColorVec4(ImGuiCol_Text)}; }
  }
  checkPhase();
  const auto vp_pos =
      ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable ? ImGui::GetMainViewport()->Pos : ImVec2{0, 0};
  const auto vp_size = ImGui::GetMainViewport()->Size;
  ImGui::SetNextWindowPos(vp_pos + ImVec2(vp_size.x - PADDING_X, vp_size.y - PADDING_Y - height), ImGuiCond_Always,
                          ImVec2(1.0f, 1.0f));

  const auto opacity = getOpacity();
  ImGui::SetNextWindowBgAlpha(opacity);

  ScopeExit end{&ImGui::End};
  if (ImGui::Begin(fmt::format("##{}", getName()).c_str(), nullptr, flags)) {
    ImGui::PushTextWrapPos(vp_size.x / 3.f);
    ScopeExit endTextWrap{&ImGui::PopTextWrapPos};

    if (icon != nullptr) {
      [[maybe_unused]] auto iconFontScoped = iconFont.applyScopedIfNotDefault();
      ImGui::PushStyleColor(ImGuiCol_Text, static_cast<ImU32>(*iconColor));
      ScopeExit popColor{[&] { ImGui::PopStyleColor(1); }};
      ImGui::Text("%s", icon);
      ImGui::SameLine();
    }
    ImGui::Text("%s", label->get().c_str());
    /*if (was_title_rendered && !NOTIFY_NULL_OR_EMPTY(content)) {
        SetCursorPosY(GetCursorPosY() + 5.f);// Must be a better way to do this!!!!
      }*/
    ImGui::Separator();
    std::ranges::for_each(getChildren(), &Renderable::render);
  }
  height += ImGui::GetWindowHeight() + PADDING_MESSAGE_Y;
}

void Notification::setIcon(const char *newIcon, Font newIconFont) {
  icon = newIcon;
  iconFont = newIconFont;
}

void Notification::setIconColor(Color newColor) { iconColor = newColor; }

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
  using namespace std::chrono;
  const auto timeElapsed = duration_cast<milliseconds>(steady_clock::now() - creationTime);
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

}  // namespace pf::ui::ig
