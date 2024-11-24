#include "rendering_device.hpp"
#include "vulkan_impl/vk_device.hpp"
#include "core/logging/logger.hpp"

namespace cannele::rhi
{
    auto RenderingDevice::create_device(DeviceType type, bool enable_validation) -> RenderingDevice*
    {
        switch (type) {
            case DeviceType::vulkan: return VulkanDevice::create_device(enable_validation);
            default: CN_CRTICAL("{0} is not support now!", type);
        }
    }
}
