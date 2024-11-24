#pragma once

namespace cannele::rhi
{
    enum struct DeviceType
    {
        vulkan,
        d3d12,
    };

    struct Extent2D final
    {
        float width{};
        float height{};
    };
}
