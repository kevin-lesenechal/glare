cmake_minimum_required(VERSION 3.10)

project(glare LANGUAGES CXX VERSION 1.0.0)

#---- OPTIONS -----------------------------------------------------------------#

set(GIT_EXECUTABLE "git" CACHE FILEPATH "git executable file or command")

#---- SOURCES -----------------------------------------------------------------#

set(SOURCES
    include/glare/opengl/buffer.hpp
    include/glare/shader/shader.hpp
    include/glare/shader/shader_program.hpp
    include/glare/opengl/vertex_array.hpp
    include/glare/material/texture.hpp
    include/glare/primitive/axis.hpp
    include/glare/shader/shader_preprocessor.hpp
    include/glare/shader/shader_source_loader_interface.hpp
    include/glare/shader/file_shader_source_loader.hpp
    include/glare/freefly_camera_controller.hpp
    include/glare/ui/debug_window.hpp
    include/glare/ui/checkbox.hpp
    include/glare/ui/ui_context.hpp
    include/glare/ui/light_config.hpp
    include/glare/ui/camera_config.hpp
    include/glare/camera.hpp
    include/glare/model/mesh.hpp
    include/glare/model/model_loader.hpp
    include/glare/model/model.hpp
    include/glare/opengl/context.hpp
    include/glare/material/texture_loader.hpp
    include/glare/material/texture_library.hpp
    include/glare/material/texture_library_interface.hpp
    include/glare/material/material.hpp
    include/glare/shader/phong_shading.hpp
    include/glare/shader/shading_interface.hpp
    include/glare/material/ktx_loader.hpp
    include/glare/ui/widget/color_temp_picker.hpp
    include/glare/material/color_utils.hpp
    include/glare/opengl/extensions.hpp
    include/glare/sizes.hpp
    include/glare/points.hpp
    include/glare/primitive/unit_cube.hpp
    include/glare/skybox.hpp
    include/glare/logger_interface.hpp
    include/glare/abstract_logger.hpp
    include/glare/null_logger.hpp
    include/glare/shader/glsl_types.hpp
    include/glare/input/keyboard_event.hpp
    include/glare/input/mouse_move_event.hpp
    include/glare/input/mouse_scroll_event.hpp
    include/glare/input/mouse_button_event.hpp
    include/glare/opengl/framebuffer.hpp
    include/glare/opengl/renderbuffer.hpp
    include/glare/hdr_renderer.hpp

    src/shader/shader_program.cpp
    src/shader/shader.cpp
    src/opengl/vertex_array.cpp
    src/material/texture.cpp
    src/primitive/axis.cpp
    src/shader/shader_preprocessor.cpp
    src/shader/file_shader_source_loader.cpp
    src/freefly_camera_controller.cpp
    src/ui/debug_window.cpp
    src/ui/checkbox.cpp
    src/ui/ui_context.cpp
    src/ui/light_config.cpp
    src/camera.cpp
    src/ui/camera_config.cpp
    src/model/mesh.cpp
    src/model/model_loader.cpp
    src/model/model.cpp
    src/opengl/context.cpp
    src/material/texture_loader.cpp
    src/material/texture_library.cpp
    src/shader/phong_shading.cpp
    src/material/ktx_loader.cpp
    src/ui/widget/color_temp_picker.cpp
    src/material/color_utils.cpp
    src/primitive/unit_cube.cpp
    src/skybox.cpp
    src/shader/glsl_types.cpp
    src/opengl/framebuffer.cpp
    src/opengl/renderbuffer.cpp
    src/hdr_renderer.cpp
)

set(TEST_SOURCES
    tests/shader/shader_preprocessor_test.cpp)

#---- BUILD OPTIONS -----------------------------------------------------------#

set(DEBUG_FLAGS         "-pedantic" "-Wall" "-Wextra")
set(OPTIM_DEBUG_FLAGS   "-O3")
set(RELEASE_FLAGS       "-O3" "-s")

#---- LIBRARIES ---------------------------------------------------------------#

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)

set(APP_LIBS
    OpenGL::OpenGL
    glfw
    epoxy
    assimp)

#------------------------------------------------------------------------------#

include(cmake/buildconfig.cmake)

set(APP_SOURCES ${SOURCES})
list(REMOVE_ITEM APP_SOURCES "src/app/main.cpp")

option(PACKAGE_TESTS "Build the tests" ON)
if (PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

#---- OUTPUT ------------------------------------------------------------------#

add_subdirectory(libs/gl_tables)
add_subdirectory(libs/simple_ktx)
add_subdirectory(libs/imgui)
add_subdirectory(libs/stb_image)
add_subdirectory(tools/ktx_export)
add_subdirectory(tools/ktx_viewer)

add_library(glare ${SOURCES})
target_link_libraries(glare
    ${APP_LIBS}
    gl_tables
    simple_ktx
    imgui
    stb_image
)
target_include_directories(glare
    PUBLIC include
    PUBLIC ${CMAKE_BINARY_DIR}/include
    PUBLIC libs/gl_tables/include
    PUBLIC libs/simple_ktx/include
    PUBLIC libs/imgui/include
    PUBLIC libs/stb_image/include
)
set_target_properties(glare PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    INTERPROCEDURAL_OPTIMIZATION YES
)
target_compile_options(glare
    PUBLIC "$<$<CONFIG:Debug>:${DEBUG_FLAGS}>"
    PUBLIC "$<$<CONFIG:RelWithDebInfo>:${OPTIM_DEBUG_FLAGS}>"
    PUBLIC "$<$<CONFIG:Release>:${RELEASE_FLAGS}>"
)
