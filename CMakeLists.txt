cmake_minimum_required(VERSION 3.18)

project(ft_ping)
set(CMAKE_C_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

set(COMMON_COMPILE_OPTIONS -Ofast -Wall -Wextra -Werror)

if(DEBUG)
    message(STATUS "Building in debug mode")
    target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_COMPILE_OPTIONS} -g3 -fsanitize=address)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
else()
    message(STATUS "Building in release mode")
    target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_COMPILE_OPTIONS})
endif()


