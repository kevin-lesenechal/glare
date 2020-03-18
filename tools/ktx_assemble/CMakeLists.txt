set(SOURCES
    main.cpp)

add_executable(ktx_assemble ${SOURCES})
target_link_libraries(
    ktx_assemble
    boost_program_options
    gl_tables
)
