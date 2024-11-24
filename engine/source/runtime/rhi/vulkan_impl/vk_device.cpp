#include "vk_device.hpp"

// #include "volk.h"

#include <iostream>
#include <vector>

namespace cannele::rhi
{
    inline namespace
    {
        VKAPI_ATTR auto VKAPI_CALL vk_debug_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
            VkDebugUtilsMessageTypeFlagsEXT message_type,
            VkDebugUtilsMessengerCallbackDataEXT const* p_callback_data,
            void* p_user_data
        ) -> VkBool32 {
            std::cerr << "[validation layer] " << p_callback_data->pMessage << std::endl;
            return VK_FALSE;
        }

        VkDebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = vk_debug_callback,
            .pUserData = nullptr,
        };

        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
            } else {
                return VK_ERROR_EXTENSION_NOT_PRESENT;
            }
        }

        void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }
    }

    auto VulkanDevice::create_device(bool enable_validation) -> VulkanDevice*
    {
        return nullptr;
    }

    VulkanDevice::VulkanDevice(bool enable_validation)
    {
        // volkInitialize();

        VkApplicationInfo app_info{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "cannele",
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName = "cannele",
            .engineVersion = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion = VK_API_VERSION_1_3,
        };

        VkDebugUtilsMessengerCreateInfoEXT debug_utils_ci{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .flags = 0,
            .messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = vk_debug_callback,
            .pUserData = nullptr,
        };

        std::vector<char const*> enabled_layers{};
        if (enable_validation) {
            enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        std::vector<char const*> enabled_instance_extensions{
            VK_KHR_SURFACE_EXTENSION_NAME,
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
            VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        };

        VkInstanceCreateInfo instance_ci{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<uint32_t>(enabled_layers.size()),
            .ppEnabledLayerNames = enabled_layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(enabled_instance_extensions.size()),
            .ppEnabledExtensionNames = enabled_instance_extensions.data(),
        };

        vkCreateInstance(&instance_ci, nullptr, &instance_);
        // volkLoadInstanceOnly(instance_);

        if (enable_validation) {
            CreateDebugUtilsMessengerEXT(instance_, &debug_utils_messenger_create_info, nullptr, &debug_utils_messenger_);
        }

        pick_physical_device();

        float queue_priority = 1.0f;
        std::vector<uint32_t> created_queue_family{queue_family_indices_[static_cast<uint8_t>(QueueType::graphics)]};
        if (queue_family_indices_[static_cast<uint8_t>(QueueType::transfer)] != ~0u) {
            created_queue_family.push_back(queue_family_indices_[static_cast<uint8_t>(QueueType::transfer)]);
        } else {
            queue_family_indices_[static_cast<uint8_t>(QueueType::transfer)] = queue_family_indices_[static_cast<uint8_t>(QueueType::graphics)];
        }
        if (queue_family_indices_[static_cast<uint8_t>(QueueType::compute)] != ~0u) {
            created_queue_family.push_back(queue_family_indices_[static_cast<uint8_t>(QueueType::compute)]);
        } else {
            queue_family_indices_[static_cast<uint8_t>(QueueType::compute)] = queue_family_indices_[static_cast<uint8_t>(QueueType::graphics)];
        }
        std::vector<VkDeviceQueueCreateInfo> queue_ci(created_queue_family.size());
        for (size_t i = 0; i < queue_ci.size(); i++) {
            queue_ci[i] = VkDeviceQueueCreateInfo{
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .queueFamilyIndex = created_queue_family[i],
                .queueCount = 1,
                .pQueuePriorities = &queue_priority,
            };
        }

        std::vector<char const*> enabled_device_extensions{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME,
        };

        VkPhysicalDeviceFeatures2 device_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = nullptr,
        };
        VkPhysicalDeviceVulkan11Features vk11_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
        };
        VkPhysicalDeviceVulkan12Features vk12_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        };
        VkPhysicalDeviceVulkan13Features vk13_features{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        };
        device_features.pNext = &vk11_features;
        vk11_features.pNext = &vk12_features;
        vk12_features.pNext = &vk13_features;

        vkGetPhysicalDeviceFeatures2(physical_device_, &device_features);

        VkDeviceCreateInfo device_create_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = static_cast<uint32_t>(queue_ci.size()),
            .pQueueCreateInfos = queue_ci.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<uint32_t>(enabled_device_extensions.size()),
            .ppEnabledExtensionNames = enabled_device_extensions.data(),
            .pEnabledFeatures = nullptr,
        };
        device_create_info.pNext = &device_features;

        vkCreateDevice(physical_device_, &device_create_info, nullptr, &device_);
        // volkLoadDevice(device_);

        for (size_t i = 0; i < 3; i++) {
            if (queue_family_indices_[i] != ~0u) {
                vkGetDeviceQueue(device_, queue_family_indices_[i], 0, &queues_[i]);
            }
        }
    }

    VulkanDevice::~VulkanDevice()
    {
        vkDestroyDevice(device_, nullptr);

        if (debug_utils_messenger_ != VK_NULL_HANDLE) {
            DestroyDebugUtilsMessengerEXT(instance_, debug_utils_messenger_, nullptr);
        }

        vkDestroyInstance(instance_, nullptr);
    }

    auto VulkanDevice::pick_physical_device() -> void
    {
        uint32_t num_physical_device;
        vkEnumeratePhysicalDevices(instance_, &num_physical_device, nullptr);
        std::vector<VkPhysicalDevice> physical_devices(num_physical_device);
        vkEnumeratePhysicalDevices(instance_, &num_physical_device, physical_devices.data());

        VkPhysicalDevice available_device = VK_NULL_HANDLE;
        for (auto physical_device : physical_devices) {
            available_device = physical_device;
            VkPhysicalDeviceProperties physical_device_props;
            vkGetPhysicalDeviceProperties(physical_device, &physical_device_props);
            if (physical_device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                physical_device_ = physical_device;
                physical_device_props_ = physical_device_props;
            }
        }
        if (physical_device_ == VK_NULL_HANDLE) {
            if (available_device == VK_NULL_HANDLE) {
                throw std::runtime_error("no available GPU found");
            }
            physical_device_ = available_device;
            vkGetPhysicalDeviceProperties(physical_device_, &physical_device_props_);
        }

        uint32_t num_queue_family;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &num_queue_family, nullptr);
        std::vector<VkQueueFamilyProperties> queue_family_props(num_queue_family);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &num_queue_family, queue_family_props.data());

        uint32_t graphics_queue_family = ~0u;
        uint32_t transfer_queue_family = ~0u;
        uint32_t compute_queue_family = ~0u;
        for (uint32_t i = 0; i < num_queue_family; i++) {
            const auto &props = queue_family_props[i];
            if (graphics_queue_family == ~0u && (props.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                graphics_queue_family = i;
            } else if (transfer_queue_family == ~0u && (props.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) {
                transfer_queue_family = i;
            } else if (compute_queue_family == ~0u && (props.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) {
                compute_queue_family = i;
            }
        }
        queue_family_indices_[static_cast<uint8_t>(QueueType::graphics)] = graphics_queue_family;
        queue_family_indices_[static_cast<uint8_t>(QueueType::compute)] = compute_queue_family;
        queue_family_indices_[static_cast<uint8_t>(QueueType::transfer)] = transfer_queue_family;
    }
}
