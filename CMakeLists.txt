cmake_minimum_required(VERSION 3.18)

project(ft_ping)
set(CMAKE_C_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/inc)
file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

if(DEBUG)
    message(STATUS "Building in debug mode")
    target_compile_options(${PROJECT_NAME} PRIVATE -g3 -fsanitize=address -Ofast)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
else()
    message(STATUS "Building in release mode")
    target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
endif()

if(BONUS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BONUS=1)
    message(STATUS "Bonus part enabled")
endif()


add_custom_target(fclean COMMAND ${CMAKE_COMMAND} -P CMakeFiles/${PROJECT_NAME}.dir/cmake_clean.cmake
                  COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_NAME})
