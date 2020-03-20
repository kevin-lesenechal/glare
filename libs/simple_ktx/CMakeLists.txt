set(SOURCES
    include/simple_ktx/simple_ktx.hpp
    include/simple_ktx/ktx_image_base_iterator.hpp
    include/simple_ktx/ktx_image_reader.hpp
    include/simple_ktx/ktx_image_writer.hpp
    src/simple_ktx.cpp
    src/ktx_image_base_iterator.cpp
    src/ktx_image_reader.cpp
    src/ktx_image_writer.cpp
    include/simple_ktx.hpp)

include_directories(include)

add_library(simple_ktx ${SOURCES})
target_link_libraries(simple_ktx gl_tables)
