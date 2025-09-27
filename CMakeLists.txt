cmake_minimum_required(VERSION 3.10)

project(loxjpp VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES src/*.cpp src/*.hpp)
add_executable(loxjpp ${SRC_FILES})

set(RESOURCE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
set(RESOURCE_DEST_DIR "$<TARGET_FILE_DIR:loxjpp>/res")

add_custom_command(
    TARGET loxjpp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ARGS ${RESOURCE_SOURCE_DIR} ${RESOURCE_DEST_DIR}
    COMMENT "Copying resources directory 'res' to the executable output folder."
)
