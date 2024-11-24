#pragma once

#include <compare>

namespace cannele::core::prelude
{
    #define CN_CUSTOM_UNIQUE(STRUCT) \
        STRUCT() = default; \
        STRUCT(STRUCT const&) = delete; \
        using Marker_Unique = STRUCT

    #define CN_UNIQUE(STRUCT) \
        STRUCT(STRUCT&&) = default; \
        auto operator=(STRUCT const&) -> STRUCT& = delete; \
        auto operator=(STRUCT&&) -> STRUCT& = default; \
        CN_CUSTOM_UNIQUE(STRUCT)

    #define CN_INTERFACE(STRUCT) \
        virtual ~STRUCT() = default; \
        CN_UNIQUE(STRUCT); \
        using Marker_Interface = STRUCT
}
