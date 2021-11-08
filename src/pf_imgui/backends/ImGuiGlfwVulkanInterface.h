//
// Created by petr on 6/27/21.
//

#ifndef PF_IMGUI_SRC_PF_IMGUI_BACKENDS_IMGUIGLFWVULKANINTERFACE_H
#define PF_IMGUI_SRC_PF_IMGUI_BACKENDS_IMGUIGLFWVULKANINTERFACE_H

#include <GLFW/glfw3.h>
#include <optional>
#include <pf_imgui/ImGuiInterface.h>
#include <vulkan/vulkan.h>

namespace pf::ui::ig {
struct ImGuiVulkanGlfwConfig {
  VkInstance instance;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkRenderPass renderPass;
  VkSurfaceKHR surface;
  VkSwapchainKHR swapchain;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  std::uint32_t swapchainImageCount;
  GLFWwindow *handle;
  ImGuiConfigFlags flags = {};
  bool enableMultiViewport = false;
  toml::table config;
  std::filesystem::path pathToIconFolder;
  Flags<IconPack> enabledIconPacks = Flags<IconPack>{};
  float defaultFontSize = 13.f;
};
class ImGuiGlfwVulkanInterface : public ImGuiInterface {
 public:
  explicit ImGuiGlfwVulkanInterface(ImGuiVulkanGlfwConfig config);
  ~ImGuiGlfwVulkanInterface() override;

  void updateFonts() override;
  void addToCommandBuffer(VkCommandBuffer commandBuffer);
  void render() override;

 protected:
  void renderImpl() override;

 private:
  std::optional<std::uint32_t> findGraphicsFamilyIndex();
  void setupDescriptorPool();
  VkDescriptorPool descriptorPool;
  ImGuiVulkanGlfwConfig config;
};
}// namespace pf::ui::ig
#endif//PF_IMGUI_SRC_PF_IMGUI_BACKENDS_IMGUIGLFWVULKANINTERFACE_H
