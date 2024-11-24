#pragma once

#include "core/prelude/object.hpp"

namespace cannele::rhi
{
    struct Shader
    {
        CN_INTERFACE(Shader);

        virtual auto module() -> void = 0;
    };
}
