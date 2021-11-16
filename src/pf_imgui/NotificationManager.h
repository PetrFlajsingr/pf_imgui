//
// Created by xflajs00 on 16.11.2021.
//

#ifndef PF_IMGUI_NOTIFICATIONMANAGER_H
#define PF_IMGUI_NOTIFICATIONMANAGER_H

#include <pf_imgui/dialogs/Notification.h>
#include <vector>

namespace pf::ui::ig {

/*
enum class NotificationType {
  Success = ImGuiToastType_Success,
  Warning = ImGuiToastType_Warning,
  Error = ImGuiToastType_Error,
  Info = ImGuiToastType_Info
};*/

class NotificationManager {
 public:
  [[nodiscard]] Notification &createNotification(const std::string &name, const std::string &label,
                                                 std::chrono::milliseconds duration = Notification::DEFAULT_DURATION);

  void renderNotifications();

 private:
  std::vector<std::unique_ptr<Notification>> notifications;
  std::vector<std::unique_ptr<Notification>> newNotifications;
};

}// namespace pf::ui::ig
#endif//PF_IMGUI_NOTIFICATIONMANAGER_H
