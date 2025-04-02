cmake_minimum_required(VERSION 3.10)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}vendor)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ATLAS
    LANGUAGES CXX C
    DESCRIPTION "An indispensable online webinar open course for computer graphics beginners to learn every bit of modern OpenGL and build a popular 3D game engine."
    HOMEPAGE_URL https://github.com/parallel101/opengltutor)

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "The binary directory of CMake cannot be the same as source directory! "
        "Do not use 'cmake .'! Please consider use 'cmake -B build' to specify a different binary directory. "
        "Otherwise this project may fail to build, or make Git hard to exclude binary files. "
        "For Windows, it is highly recommended to use Visual Studio as IDE for CMake projects.")
endif()

message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

if (NOT MSVC)
    find_program(CCACHE_PROGRAM ccache)  # Compilation speed up: https://ccache.dev
    if (CCACHE_PROGRAM)
        message(STATUS "Found CCache: ${CCACHE_PROGRAM}")
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
    endif()
endif()

if (MSVC)
    # FUCKING VS2022
    include_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/include)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/execution-charset:utf-8>")
endif()

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS include/*.h include/*.hpp)
add_executable(${PROJECT_NAME} ${sources} ${headers})

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/vendor/stb_image)
include_directories(${PROJECT_SOURCE_DIR}/vendor/spdlog/include)

add_subdirectory(${VENDOR_DIR}/glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

add_subdirectory(${VENDOR_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(${VENDOR_DIR}/glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(${VENDOR_DIR}/imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

target_compile_definitions(${PROJECT_NAME} PUBLIC -DOPENGLTUTOR_HOME=\"${CMAKE_CURRENT_SOURCE_DIR}/\")