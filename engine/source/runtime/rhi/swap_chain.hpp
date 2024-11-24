#pragma once

#include "core/prelude/object.hpp"
#include "core/span.hpp"
#include "defines.hpp"
#include "sync.hpp"

namespace cannele::rhi
{
    struct SwapChain
    {
        CN_INTERFACE(SwapChain);

        virtual auto resize(Extent2D extent) -> void = 0;

        virtual auto present(core::CSpan<Semaphore> wait_semaphores = {}) -> void = 0;
    };
}
