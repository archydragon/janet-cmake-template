cmake_minimum_required(VERSION 3.21)
project(janet_cmake_template)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

# Janet.
set(JANET_SRC janet/src)
file(GLOB JANET_SRC_FILES ${JANET_SRC}/core/*.c)
file(GLOB JANET_BOOT_FILES ${JANET_SRC}/boot/*.c)

if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -fvisibility=hidden -fPIC -O0 -g")
endif()

# Version identification.
file(READ ${JANET_SRC}/conf/janetconf.h _janetconf_h_contents)
string(REGEX REPLACE ".*#define[ \t]+JANET_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
    "\\1" JANET_FULL_VERSION ${_janetconf_h_contents})

# Bootstrap.
add_definitions(-DJANET_BOOTSTRAP)

# Put bootstrap files to a separate directory, as wo don't need them in the main tree.
set(JANET_BOOTSTRAP_DIR ${CMAKE_CURRENT_BINARY_DIR}/janet-boot)
add_executable(janet_boot
    ${JANET_SRC_FILES}
    ${JANET_BOOT_FILES})
target_include_directories(janet_boot PRIVATE
    ${JANET_SRC}/include
    ${JANET_SRC}/conf)
set_target_properties(janet_boot PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${JANET_BOOTSTRAP_DIR})

if(UNIX)
    target_link_libraries(janet_boot m pthread)
endif()

# Amalgamation.
file(COPY ${JANET_SRC} DESTINATION ${JANET_BOOTSTRAP_DIR})
set(JANET_AMALG_OUTPUT ${JANET_BOOTSTRAP_DIR}/janet.c)
add_custom_command(
    COMMAND janet_boot ${JANET_BOOTSTRAP_DIR} > ${JANET_AMALG_OUTPUT}
    OUTPUT ${JANET_AMALG_OUTPUT})
add_custom_target(janet_amalg ALL
    DEPENDS ${JANET_AMALG_OUTPUT})

# Final library.
if(JANET_STATIC)
    add_library(janet STATIC
        ${JANET_AMALG_OUTPUT}
        ${JANET_INCLUDES})
else()
    add_library(janet SHARED
        ${JANET_AMALG_OUTPUT}
        ${JANET_INCLUDES})
endif()

target_include_directories(janet PUBLIC
    ${JANET_SRC}/include
    ${JANET_SRC}/conf)

if(NOT CYGWIN)
    set_target_properties(janet PROPERTIES VERSION ${JANET_FULL_VERSION})
endif()
if(UNIX)
    set_target_properties(janet PROPERTIES SOVERSION 1)
    target_link_libraries(janet m pthread dl)
endif()

# Application binary.
add_executable(janet_cmake_template main.cpp)
target_link_libraries(janet_cmake_template PUBLIC janet)
