//
// Created by petr on 6/27/21.
//

#include "ImGuiGlfwVulkanInterface.h"
#include <src/pf_imgui/backends/impl/imgui_impl_glfw.h>
#include <src/pf_imgui/backends/impl/imgui_impl_vulkan.h>

namespace pf::ui::ig {
namespace details {
void checkVkResult(VkResult err) {
  if (err == 0) { return; }
  if (err < 0) { throw std::runtime_error("Error: VkResult = " + std::to_string(err)); }
}
}  // namespace details

ImGuiGlfwVulkanInterface::ImGuiGlfwVulkanInterface(ImGuiVulkanGlfwConfig config)
    : ImGuiInterface(std::move(config.imgui)), config(config) {
  setupDescriptorPool();
  ImGui_ImplGlfw_InitForVulkan(config.handle, true);
  auto init_info = ImGui_ImplVulkan_InitInfo();
  init_info.Instance = config.instance;
  init_info.PhysicalDevice = config.physicalDevice;
  init_info.Device = config.device;
  const auto graphFamilyIdx = findGraphicsFamilyIndex();
  if (!graphFamilyIdx.has_value()) { throw std::runtime_error("Couldn't find graphics family queue index"); }
  init_info.QueueFamily = *graphFamilyIdx;
  init_info.Queue = config.presentQueue;
  init_info.PipelineCache = VK_NULL_HANDLE;
  init_info.DescriptorPool = descriptorPool;
  init_info.Allocator = nullptr;
  init_info.MinImageCount = config.swapchainImageCount;
  init_info.ImageCount = config.swapchainImageCount;
  init_info.CheckVkResultFn = details::checkVkResult;
  ImGui_ImplVulkan_Init(&init_info, config.renderPass);

  updateFonts();
}

ImGuiGlfwVulkanInterface::~ImGuiGlfwVulkanInterface() {
  vkDeviceWaitIdle(config.device);
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  vkDestroyDescriptorPool(config.device, descriptorPool, nullptr);
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
  cmdBufferConfig.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  cmdBufferConfig.commandPool = commandPool;
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

void ImGuiGlfwVulkanInterface::setupDescriptorPool() {
  constexpr auto DESCRIPTOR_COUNT = 1000;
  auto descPoolConfig = VkDescriptorPoolCreateInfo{};
  descPoolConfig.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  auto poolSizes = std::vector<VkDescriptorPoolSize>{};
  poolSizes = {{VK_DESCRIPTOR_TYPE_SAMPLER, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, DESCRIPTOR_COUNT},
               {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, DESCRIPTOR_COUNT}};
  descPoolConfig.poolSizeCount = poolSizes.size();
  descPoolConfig.pPoolSizes = poolSizes.data();
  descPoolConfig.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  descPoolConfig.maxSets = DESCRIPTOR_COUNT * poolSizes.size();
  if (vkCreateDescriptorPool(config.device, &descPoolConfig, nullptr, &descriptorPool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

void ImGuiGlfwVulkanInterface::newFrame_impl() {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
}

void ImGuiGlfwVulkanInterface::renderDrawData_impl(ImDrawData *drawData) {}

}  // namespace pf::ui::ig