cmake_minimum_required(VERSION 3.10)

project(glare LANGUAGES CXX VERSION 1.0.0)

#---- OPTIONS -----------------------------------------------------------------#

set(GIT_EXECUTABLE "git" CACHE FILEPATH "git executable file or command")

#---- SOURCES -----------------------------------------------------------------#

set(SOURCES
    src/third_party/stb_image.cpp
    src/third_party/imgui.cpp
    src/third_party/imgui_impl_glfw.cpp
    src/third_party/imgui_impl_opengl3.cpp
    src/third_party/imgui_draw.cpp
    src/third_party/imgui_widgets.cpp

    include/glare/opengl/buffer.hpp
    include/glare/shader/shader.hpp
    include/glare/shader/shader_program.hpp
    include/glare/opengl/vertex_array.hpp
    include/glare/opengl/texture.hpp
    include/glare/primitive/axis.hpp
    include/glare/shader/shader_preprocessor.hpp
    include/glare/shader/shader_source_loader_interface.hpp
    include/glare/shader/fs_shader_source_loader.hpp
    include/glare/fps_camera.hpp
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

    src/shader/shader_program.cpp
    src/shader/shader.cpp
    src/opengl/vertex_array.cpp
    src/opengl/texture.cpp
    src/primitive/axis.cpp
    src/shader/shader_preprocessor.cpp
    src/shader/fs_shader_source_loader.cpp
    src/fps_camera.cpp
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
)

set(TEST_SOURCES
    tests/shader/shader_preprocessor_test.cpp)

#---- BUILD OPTIONS -----------------------------------------------------------#

include_directories(include)
include_directories(include/imgui)
include_directories(${CMAKE_BINARY_DIR}/include)

set(CMAKE_CXX_STANDARD      17)
set(CMAKE_CXX_FLAGS         "-std=c++17 -pedantic -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG   "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-s -O3 -flto")

#---- LIBRARIES ---------------------------------------------------------------#

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(glfw3 3.3 REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(assimp REQUIRED)

set(APP_LIBS
    OpenGL::OpenGL
    glfw
    Boost::filesystem
    epoxy
    assimp)

#------------------------------------------------------------------------------#

include(cmake/buildconfig.cmake)
#include(cmake/tests.cmake)

set(APP_SOURCES ${SOURCES})
list(REMOVE_ITEM APP_SOURCES "src/app/main.cpp")
#add_executable(tests ${TEST_SOURCES} ${APP_SOURCES})
#target_link_libraries(tests gtest_main)

#---- OUTPUT ------------------------------------------------------------------#

add_subdirectory(libs/gl_tables)
add_subdirectory(tools/ktx_assemble)

add_library(glare ${SOURCES})
target_link_libraries(glare ${APP_LIBS} gl_tables)
