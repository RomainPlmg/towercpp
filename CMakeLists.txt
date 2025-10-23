cmake_minimum_required(VERSION 3.20.0)
project(towercpp
    VERSION 0.1.0
    DESCRIPTION "Language Server Protocol (LSP) implementation in C++20"
    LANGUAGES CXX)

# ============================================================================
# Build options
# ============================================================================
option(TOWERCPP_BUILD_SHARED "Build shared library" OFF)
option(TOWERCPP_BUILD_EXAMPLES "Build examples" OFF)
option(TOWERCPP_BUILD_TESTS "Build tests" OFF)
option(TOWERCPP_BUILD_DOC "Build documentation" OFF)

# ============================================================================
# C++ Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_command.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Define library
# ============================================================================
file(GLOB_RECURSE TOWERCPP_SRC
    "src/*.cpp"
)

file(GLOB_RECURSE TOWERCPP_HEADERS
    "include/towercpp/*.hpp"
)

if (MAINR_BUILD_SHARED)
    add_library(towercpp SHARED ${TOWERCPP_SRC} ${TOWERCPP_HEADERS})
else()
    add_library(towercpp STATIC ${TOWERCPP_SRC} ${TOWERCPP_HEADERS})
endif()

# Alias
add_library(towercpp::towercpp ALIAS towercpp)

# ============================================================================
# Include directories (REQUIRED for portability)
# ============================================================================
target_include_directories(towercpp
    PUBLIC
        # Build from source
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # Use after install
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================================================
# Dependencies
# ============================================================================
include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG develop
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)

target_link_libraries(towercpp PUBLIC nlohmann_json::nlohmann_json spdlog::spdlog)

# ============================================================================
# Compilation properties
# ============================================================================
target_compile_features(towercpp PUBLIC cxx_std_20)

# Warnings
if(MSVC)
    target_compile_options(towercpp PRIVATE /W4)
else()
    target_compile_options(towercpp PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ============================================================================
# Print configuration
# ============================================================================
message(STATUS "")
message(STATUS "=== towercpp configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Shared lib: ${TOWERCPP_BUILD_SHARED}")
message(STATUS "Build examples: ${TOWERCPP_BUILD_EXAMPLES}")
message(STATUS "Build tests: ${TOWERCPP_BUILD_TESTS}")
message(STATUS "Build documentation: ${TOWERCPP_BUILD_DOC}")
message(STATUS "============================")