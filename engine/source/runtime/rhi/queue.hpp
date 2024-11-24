#pragma once

#include "core/prelude/object.hpp"

namespace cannele::rhi
{
    enum struct QueueType
    {
        graphics,
        compute,
        transfer,
        sparse_binding,
    };

    struct Queue
    {
        CN_INTERFACE(Queue);
    };
}
