//
// Created by xflajs00 on 16.11.2021.
//

#ifndef PF_IMGUI_NOTIFICATIONMANAGER_H
#define PF_IMGUI_NOTIFICATIONMANAGER_H

#include <memory>
#include <pf_imgui/FontManager.h>
#include <pf_imgui/dialogs/Notification.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

enum class NotificationType { Success, Warning, Error, Info };

class NotificationManager {
 public:
  explicit NotificationManager(FontManager &fontManager);

  [[nodiscard]] Notification &createNotification(const std::string &name, const std::string &label,
                                                 std::chrono::milliseconds duration = Notification::DEFAULT_DURATION);

  [[nodiscard]] Notification &createNotification(NotificationType notificationType, const std::string &name,
                                                 const std::string &label,
                                                 std::chrono::milliseconds duration = Notification::DEFAULT_DURATION);

  void renderNotifications();

 private:
  std::vector<std::unique_ptr<Notification>> notifications;
  std::vector<std::unique_ptr<Notification>> newNotifications;
  ImFont *defaultFont = nullptr;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_NOTIFICATIONMANAGER_H
