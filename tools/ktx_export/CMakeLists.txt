set(SOURCES
    main.cpp
    options.hpp
    options.cpp
)

find_package(glfw3 3.3 REQUIRED)

add_executable(ktx_export ${SOURCES})
target_include_directories(ktx_export
    PRIVATE /usr/include/ImageMagick-7
)
target_link_libraries(ktx_export
    boost_program_options
    simple_ktx
    epoxy
    glfw
    Magick++-7.Q16HDRI MagickWand-7.Q16HDRI MagickCore-7.Q16HDRI
)
target_compile_definitions(ktx_export
    PUBLIC MAGICKCORE_HDRI_ENABLE=1
    PUBLIC MAGICKCORE_QUANTUM_DEPTH=16
)
