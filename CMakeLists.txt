cmake_minimum_required(VERSION 4.0.4)

project(noct.sh VERSION 0.1.0 LANGUAGES C)

option(NOCTSH_BUILTIN_CD "Enable the `cd` command" ON)
option(NOCTSH_BUILTIN_DIR "Enable the `dir` command" ON)

set(BUILTIN_FILES src/builtins/help.c)

if(NOCTSH_BUILTIN_CD)
    set(BUILTIN_FILES ${BUILTIN_FILES} src/builtins/cd.c)
endif()

if(NOCTSH_BUILTIN_DIR)
    set(BUILTIN_FILES ${BUILTIN_FILES} src/builtins/dir.c)
endif()

if(CMAKE_C_COMPILER MATCHES "-nocturne-")
    message("Compiling for NocturneOS")
    set(IMPL_FILES src/impl/nocturne/run.c)
else()
    message("Compiling for generic OS")
    set(IMPL_FILES src/impl/generic/run.c)
endif()


add_executable(noctsh
    include/noctsh_string.h

    src/main.c
    include/noctsh.h
    src/string.c
    include/noctsh_vector.h
    include/noctsh_option.h
    src/vector.c

    ${IMPL_FILES}
    ${BUILTIN_FILES}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

target_include_directories(noctsh PRIVATE ${CMAKE_SOURCE_DIR}/include)
