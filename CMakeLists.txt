cmake_minimum_required(VERSION 3.19)

project(ShulkerNBT
    VERSION 0.0.1
    LANGUAGES CXX
    HOMEPAGE_URL "https://github.com/OriginEffect/ShulkerNBT"
    DESCRIPTION "A nbt library written in c++"
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SHULKER_NBT_LIB_NAME ${PROJECT_NAME})

# ----------------------
# Build Options
# ----------------------
option(SHULKER_NBT_BUILD_STATIC "Build static library" OFF)
option(SHULKER_NBT_BUILD_TESTS "Build tests" OFF)

# ----------------------
# Add Library
# ----------------------

if(SHULKER_NBT_BUILD_STATIC)
    set(_type STATIC)
else()
    set(_type SHARED)
endif()

add_library(${SHULKER_NBT_LIB_NAME} ${_type})

target_compile_definitions(ShulkerNBT PRIVATE SHULKER_NBT_EXPORTS)
target_include_directories(${SHULKER_NBT_LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(fmt REQUIRED)
find_package(ZLIB REQUIRED)
target_link_libraries(${SHULKER_NBT_LIB_NAME}
    PRIVATE fmt::fmt
    PRIVATE ZLIB::ZLIB
)

# ----------------------
# Install
# ----------------------


# ----------------------
# Add Subdirectories
# ----------------------

add_subdirectory(src)

if(SHULKER_NBT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
