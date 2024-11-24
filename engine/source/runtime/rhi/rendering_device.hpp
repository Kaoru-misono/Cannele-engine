#pragma once

#include "core/prelude/object.hpp"
#include "defines.hpp"

namespace cannele::rhi
{
    struct RenderingDevice
    {
        CN_INTERFACE(RenderingDevice);

        static auto create_device(DeviceType type, bool enable_validation = false) -> RenderingDevice*;
    };
}
