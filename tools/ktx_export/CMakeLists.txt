set(SOURCES
    main.cpp
    options.hpp
    options.cpp
)

include_directories(/usr/include/ImageMagick-7)

add_definitions(-DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)

add_executable(ktx_export ${SOURCES})
target_link_libraries(
    ktx_export
    boost_program_options
    simple_ktx
    epoxy
    Magick++-7.Q16HDRI MagickWand-7.Q16HDRI MagickCore-7.Q16HDRI
)
