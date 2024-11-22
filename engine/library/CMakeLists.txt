set(library-floder "Library")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(NOT TARGET imgui)
    set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

    file(GLOB imgui_sources CONFIGURE_DEPENDS  "${imgui_SOURCE_DIR_}/*.cpp")
    file(GLOB imgui_impl CONFIGURE_DEPENDS
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp"
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.h"
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_vulkan.cpp"
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_vulkan.h")
    add_library(imgui STATIC ${imgui_sources} ${imgui_impl})
    target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}>)
    target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${vulkan_include}>)
    target_link_libraries(imgui PUBLIC glfw ${vulkan_lib})
    set_target_properties(imgui PROPERTIES FOLDER ${library-floder}/imgui)
endif()

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${library-floder}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${library-floder}/glfw)
endif()

if(NOT TARGET spdlog)
    option(SPDLOG_BUILD_EXAMPLE "" OFF)
    option(SPDLOG_INSTALL "" OFF)
    add_subdirectory(spdlog)
    set_target_properties(spdlog PROPERTIES FOLDER ${library-floder}/spdlog)
endif()

if(NOT TARGET stb)
    file(GLOB stb_sources CONFIGURE_DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/stb/*.h")
    add_library(stb INTERFACE ${stb_sources})
    target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)
    set_target_properties(stb PROPERTIES FOLDER ${library-floder}/stb)
endif()

if(NOT TARGET glm)
    add_library(glm INTERFACE)
    target_include_directories(glm INTERFACE glm)
endif()

if(NOT TARGET assimp)
    add_subdirectory(assimp)
    set_target_properties(assimp PROPERTIES FOLDER ${library-floder}/assimp)
endif()
