/**
* @file NotificationManager.h
* @brief Class for management of notifications.
* @author Petr Flajšingr
* @date 16.11.21
*/
#ifndef PF_IMGUI_NOTIFICATIONMANAGER_H
#define PF_IMGUI_NOTIFICATIONMANAGER_H

#include <memory>
#include <pf_imgui/_export.h>
#include <pf_imgui/dialogs/Notification.h>
#include <pf_imgui/managers/FontManager.h>
#include <string>
#include <vector>

namespace pf::ui::ig {

/**
 * @brief A class managing notification lifetime and interaction.
 * @todo: create some notification presets
 */
class PF_IMGUI_EXPORT NotificationManager {
 public:
  /**
   * Construct NotificationManager.
   */
  NotificationManager() = default;

  /**
   * Create a simple notification with given duration.
   * @param name unique name of the element
   * @param label text rendered on top of the notification
   * @param duration lifetime
   * @return reference to the new notification
   */
  [[nodiscard]] Notification &createNotification(const std::string &name, const std::string &label,
                                                 std::chrono::milliseconds duration = Notification::DEFAULT_DURATION);

 private:
  friend class ImGuiInterface;
  void renderNotifications();

  std::vector<std::unique_ptr<Notification>> notifications;
  std::vector<std::unique_ptr<Notification>> newNotifications;
};

}  // namespace pf::ui::ig
#endif  // PF_IMGUI_NOTIFICATIONMANAGER_H
