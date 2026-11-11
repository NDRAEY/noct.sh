cmake_minimum_required(VERSION 4.0.4)

project(noct.sh VERSION 0.1.0 LANGUAGES C)

add_executable(noctsh 
    include/noctsh_string.h

    src/main.c
    include/noctsh.h
    src/string.c
    include/noctsh_vector.h
    include/noctsh_option.h
    src/vector.c
)

target_include_directories(noctsh PRIVATE ${CMAKE_SOURCE_DIR}/include)