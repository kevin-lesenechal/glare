set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

set(SOURCES
    src/main.cpp
    src/mainwindow.hpp
    src/mainwindow.cpp
    src/texture_renderer.hpp
    src/texture_renderer.cpp

    ui/mainwindow.ui
)

find_package(Qt5Widgets)

add_executable(ktx_viewer ${SOURCES})
target_link_libraries(ktx_viewer
    Qt5::Widgets
    simple_ktx
    gl_tables
    epoxy
)
target_include_directories(ktx_viewer
    PRIVATE src
)
