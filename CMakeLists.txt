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

if(BONUS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE BONUS=1)
    message(STATUS "BONUS compilation flag enabled")
endif()

add_custom_target(bonus
    COMMAND ${CMAKE_COMMAND} -E env CXXFLAGS=-DBONUS=1 ${CMAKE_COMMAND} --build . --target ${PROJECT_NAME}
    COMMENT "Building with BONUS flag"
)

add_custom_target(fclean
    COMMAND ${CMAKE_COMMAND} --build . --target clean
    COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_NAME}
    COMMENT "Removing all object files and executable"
)

add_custom_target(re
    COMMAND ${CMAKE_COMMAND} --build . --target fclean
    COMMAND ${CMAKE_COMMAND} --build . --target all
    COMMENT "Rebuilding everything"
)
