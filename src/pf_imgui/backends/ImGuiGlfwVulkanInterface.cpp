//
// Created by petr on 6/27/21.
//

#include "ImGuiGlfwVulkanInterface.h"
#include <include/backends/imgui_impl_glfw.h>
#include <include/backends/imgui_impl_vulkan.h>

namespace pf::ui::ig {
ImGuiGlfwVulkanInterface::ImGuiGlfwVulkanInterface(ImGuiVulkanGlfwConfig config)
    : ImGuiInterface(config.flags, std::move(config.config), config.enableMultiViewport, config.pathToIconFolder,
                     config.enabledIconPacks, config.defaultFontSize),
      config(std::move(config)) {}

ImGuiGlfwVulkanInterface::~ImGuiGlfwVulkanInterface() {
  vkDeviceWaitIdle(config.device);
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiGlfwVulkanInterface::updateFonts() {
  const auto graphFamilyIdx = findGraphicsFamilyIndex();
  if (!graphFamilyIdx.has_value()) { throw std::runtime_error("Couldn't find graphics family queue index"); }
  auto cmdPoolConfig = VkCommandPoolCreateInfo{};
  cmdPoolConfig.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cmdPoolConfig.queueFamilyIndex = *graphFamilyIdx;
  cmdPoolConfig.flags = 0;
  auto commandPool = VkCommandPool{};

  if (vkCreateCommandPool(config.device, &cmdPoolConfig, nullptr, &commandPool) != VK_SUCCESS) {
    throw std::runtime_error("Couldn't create command pool");
  }

  auto cmdBufferConfig = VkCommandBufferAllocateInfo{};
  cmdBufferConfig.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cmdBufferConfig.commandBufferCount = 1;
  cmdBufferConfig.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  auto commandBuffer = VkCommandBuffer{};
  if (vkAllocateCommandBuffers(config.device, &cmdBufferConfig, &commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
  auto beginInfo = VkCommandBufferBeginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;
  beginInfo.pInheritanceInfo = nullptr;

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }
  ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) { throw std::runtime_error("failed to record command buffer!"); }

  auto fence = VkFence{};
  auto fenceConfig = VkFenceCreateInfo{};
  fenceConfig.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceConfig.pNext = nullptr;
  fenceConfig.flags = 0;
  if (vkCreateFence(config.device, &fenceConfig, nullptr, &fence) != VK_SUCCESS) {
    throw std::runtime_error("Fence creation failed");
  }
  auto submitInfo = VkSubmitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;
  submitInfo.signalSemaphoreCount = 0;
  submitInfo.waitSemaphoreCount = 0;
  if (vkQueueSubmit(config.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }
  vkQueueWaitIdle(config.graphicsQueue);
  vkDeviceWaitIdle(config.device);

  vkDestroyFence(config.device, fence, nullptr);
  vkDestroyCommandPool(config.device, commandPool, nullptr);
}
std::optional<std::uint32_t> ImGuiGlfwVulkanInterface::findGraphicsFamilyIndex() {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(config.physicalDevice, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(config.physicalDevice, &queueFamilyCount, queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { return i; }
  }

  return std::nullopt;
}
void ImGuiGlfwVulkanInterface::addToCommandBuffer(VkCommandBuffer commandBuffer) {
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}
void ImGuiGlfwVulkanInterface::render() {
  if (shouldUpdateFontAtlas) {
    shouldUpdateFontAtlas = false;
    updateFonts();
  }
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (getVisibility() == Visibility::Visible) {
    if (getEnabled() == Enabled::No) {
      ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
      ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
      auto raiiEnabled = pf::RAII([] {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
      });
      renderImpl();
    } else {
      renderImpl();
    }
  }
  ImGui::Render();
  if (getIo().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }
}
void ImGuiGlfwVulkanInterface::renderImpl() {
  if (hasMenuBar()) { menuBar->render(); }
  ImGuiInterface::renderImpl();
  renderDialogs();
}

}// namespace pf::ui::ig