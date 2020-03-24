set(SOURCES
    src/stb_image.cpp
)

add_library(stb_image ${SOURCES})

target_include_directories(stb_image
    PUBLIC include
)
