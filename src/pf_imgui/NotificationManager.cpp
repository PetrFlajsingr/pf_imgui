//
// Created by xflajs00 on 16.11.2021.
//

#include "NotificationManager.h"

namespace pf::ui::ig {

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
  notifications.erase(
      std::ranges::begin(std::ranges::remove(notifications, NotificationPhase::Expired,
                                             [](const auto &notification) { return notification->currentPhase; })),
      std::ranges::end(notifications));
}

}// namespace pf::ui::ig