//
// Created by xflajs00 on 16.11.2021.
//

#include "NotificationManager.h"
#include <memory>
#include <string>

namespace pf::ui::ig {

NotificationManager::NotificationManager(FontManager &fontManager)
    : defaultFont(
        fontManager.fontBuilder("def_icon_notif").addIconSubfont(IconPack::FontAwesome5Regular, 13.f).build()) {}

Notification &NotificationManager::createNotification(const std::string &name, const std::string &label,
                                                      std::chrono::milliseconds duration) {
  return *newNotifications.emplace_back(std::make_unique<Notification>(name, label, duration));
}

void NotificationManager::renderNotifications() {
  notifications.insert(notifications.end(), std::make_move_iterator(newNotifications.begin()),
                       std::make_move_iterator(newNotifications.end()));
  newNotifications.clear();
  float height = 0.f;
  std::ranges::for_each(notifications, [&](auto &notification) {
    notification->height = height;
    notification->render();
    height = notification->height;
  });
  auto remove = std::ranges::remove(notifications, NotificationPhase::Expired,
                                    [](const auto &notification) { return notification->currentPhase; });
  notifications.erase(remove.begin(), remove.end());
}

Notification &NotificationManager::createNotification(NotificationType notificationType, const std::string &name,
                                                      const std::string &label, std::chrono::milliseconds duration) {
  auto &notification = *newNotifications.emplace_back(std::make_unique<Notification>(name, label, duration));
  const char *icon = nullptr;
  Color color;
  switch (notificationType) {
    case NotificationType::Success:
      color = Color::RGB(0u, 255u, 0u, 255u);
      icon = ICON_FA_CHECK_CIRCLE;
      break;
    case NotificationType::Warning:
      color = Color::RGB(255, 255, 0, 255);
      icon = ICON_FA_EXCLAMATION_TRIANGLE;
      break;
    case NotificationType::Error:
      color = Color::RGB(255, 0, 0, 255);
      icon = ICON_FA_TIMES_CIRCLE;
      break;
    case NotificationType::Info:
      color = Color::RGB(0, 157, 255, 255);
      icon = ICON_FA_INFO_CIRCLE;
      break;
  }
  notification.setIcon(icon, defaultFont);
  notification.setIconColor(color);
  return notification;
}

}  // namespace pf::ui::ig
