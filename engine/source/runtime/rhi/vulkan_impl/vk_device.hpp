#pragma once

#include "rhi/rendering_device.hpp"
#include "rhi/queue.hpp"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
#include <array>

namespace cannele::rhi
{
    struct VulkanDevice final: RenderingDevice
    {
        static auto create_device(bool enable_validation) -> VulkanDevice*;

        VulkanDevice(bool enable_validation);
        ~VulkanDevice() override;

        auto raw() -> VkDevice { return device_; }
        auto raw_physical_device() -> VkPhysicalDevice { return physical_device_; }
        auto raw_instance() -> VkInstance { return instance_; }
        auto allocator() -> VmaAllocator { return allocator_; }

        // TODO: Move this function to private when swap chain is ready
        auto pick_physical_device() -> void;
        auto queues() -> std::array<VkQueue, 3> { return queues_; }
        VkSurfaceKHR surface_{};

    private:
        VkInstance instance_{VK_NULL_HANDLE};
        VkPhysicalDevice physical_device_{VK_NULL_HANDLE};
        VkPhysicalDeviceProperties physical_device_props_{};
        VkDevice device_{VK_NULL_HANDLE};
        VmaAllocator allocator_{nullptr};

        VkDebugUtilsMessengerEXT debug_utils_messenger_{VK_NULL_HANDLE};

        std::array<uint32_t, 3> queue_family_indices_{};
        std::array<VkQueue, 3> queues_{};
    };
}
