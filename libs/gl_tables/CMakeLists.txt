set(SOURCES
    src/tables.cpp
    src/formats.cpp
)

add_library(gl_tables ${SOURCES})

target_include_directories(gl_tables
    PUBLIC include
)
