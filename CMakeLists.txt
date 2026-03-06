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
option(SHULKER_NBT_BUILD_EXAMPLES "Build examples" OFF)
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

set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(third/fmt)
add_subdirectory(third/nlohmann_json)
target_link_libraries(${SHULKER_NBT_LIB_NAME}
    PUBLIC
    fmt::fmt
    nlohmann_json::nlohmann_json
)

# ----------------------
# Install
# ----------------------


# ----------------------
# Add Subdirectories
# ----------------------

add_subdirectory(src)

# Examples and tests
if(SHULKER_NBT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(SHULKER_NBT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
