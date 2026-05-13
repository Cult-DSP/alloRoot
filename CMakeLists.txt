cmake_minimum_required(VERSION 3.20)

project(alloRoot
    VERSION 0.1.0
    DESCRIPTION "Minimal AlloLib wrapper example for Spatial Root's EngineSessionCore API"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(SPATIAL_ROOT_DIR "" CACHE PATH "Path to a Spatial Root checkout on the devel branch")

set(_allowroot_default_spatial_root_dir "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spatialroot")
if(SPATIAL_ROOT_DIR)
    file(TO_CMAKE_PATH "${SPATIAL_ROOT_DIR}" ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR)
    set(ALLOWROOT_SPATIAL_ROOT_SOURCE "SPATIAL_ROOT_DIR cache override")
else()
    file(TO_CMAKE_PATH "${_allowroot_default_spatial_root_dir}" ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR)
    set(ALLOWROOT_SPATIAL_ROOT_SOURCE "bundled submodule")
endif()

if(NOT EXISTS "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "Spatial Root directory not found.\n"
        "Resolved path: ${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}\n"
        "Provide -DSPATIAL_ROOT_DIR=/path/to/spatialroot or initialize the submodule:\n"
        "  git submodule update --init --recursive\n"
        "Expected branch: devel"
    )
endif()

message(STATUS "alloRoot Spatial Root source: ${ALLOWROOT_SPATIAL_ROOT_SOURCE}")
message(STATUS "alloRoot Spatial Root dir: ${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}")

set(BUILD_PROGRAMS OFF CACHE BOOL "Don't build libsndfile programs" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Don't build libsndfile examples" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Don't build libsndfile tests" FORCE)
set(ENABLE_EXTERNAL_LIBS OFF CACHE BOOL "Disable libsndfile external codecs" FORCE)
set(ENABLE_MPEG OFF CACHE BOOL "Disable libsndfile MPEG support" FORCE)
add_subdirectory(
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/thirdparty/libsndfile"
    "${CMAKE_BINARY_DIR}/libsndfile-build"
)

set(SNDFILE_INCLUDE_DIR
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/thirdparty/libsndfile/include"
    CACHE PATH
    "libsndfile include dir"
    FORCE
)
set(SNDFILE_LIBRARY SndFile::sndfile CACHE STRING "libsndfile library target" FORCE)
set(SNDFILE_LIBRARIES SndFile::sndfile CACHE STRING "libsndfile library targets" FORCE)
set(SNDFILE_FOUND TRUE CACHE BOOL "libsndfile found" FORCE)

set(ALLOLIB_BUILD_EXAMPLES OFF CACHE BOOL "Disable AlloLib examples" FORCE)
set(ALLOLIB_BUILD_TESTS OFF CACHE BOOL "Disable AlloLib tests" FORCE)
set(AL_VERBOSE_OUTPUT OFF CACHE BOOL "Disable verbose AlloLib output" FORCE)
set(CULT_ALLOLIB_ENABLE_AUDIO ON CACHE BOOL "Build cult-allolib audio support" FORCE)
set(CULT_ALLOLIB_ENABLE_OSC ON CACHE BOOL "Build cult-allolib OSC support" FORCE)
set(CULT_ALLOLIB_ENABLE_PARAMETERS ON CACHE BOOL "Build cult-allolib parameter support" FORCE)
set(CULT_ALLOLIB_ENABLE_SPATIAL ON CACHE BOOL "Build cult-allolib spatial support" FORCE)
set(CULT_ALLOLIB_ENABLE_GRAPHICS ON CACHE BOOL "Build cult-allolib graphics support" FORCE)
set(CULT_ALLOLIB_ENABLE_APP ON CACHE BOOL "Build cult-allolib app support" FORCE)
set(CULT_ALLOLIB_ENABLE_VR OFF CACHE BOOL "Disable cult-allolib VR support" FORCE)
set(CULT_ALLOLIB_ENABLE_MIDI OFF CACHE BOOL "Disable cult-allolib MIDI support" FORCE)
set(CULT_ALLOLIB_ENABLE_SERIAL OFF CACHE BOOL "Disable cult-allolib serial support" FORCE)
set(CULT_ALLOLIB_ENABLE_EXAMPLES OFF CACHE BOOL "Disable cult-allolib examples" FORCE)
set(CULT_ALLOLIB_ENABLE_TESTS OFF CACHE BOOL "Disable cult-allolib tests" FORCE)
set(CULT_ALLOLIB_ENABLE_DOCS OFF CACHE BOOL "Disable cult-allolib docs" FORCE)

add_subdirectory(
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/internal/cult-allolib"
    "${CMAKE_BINARY_DIR}/cult-allolib-build"
)

# These are alloRoot-side compatibility shims for the current devel checkout.
# They do not modify Spatial Root; they only make the vendored AlloLib target
# usable as an external host dependency from this repo.
target_include_directories(cult_allolib_app PRIVATE
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/internal/cult-allolib/external/cpptoml/include"
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/internal/cult-allolib/external/rtmidi"
)
target_sources(cult_allolib_graphics PRIVATE
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/internal/cult-allolib/src/io/al_ControlNav.cpp"
)

set(SPATIALROOT_BUILD_DEVTOOLS OFF CACHE BOOL "Disable Spatial Root devtools" FORCE)
add_subdirectory(
    "${ALLOWROOT_RESOLVED_SPATIAL_ROOT_DIR}/source/spatial_engine/realtimeEngine"
    "${CMAKE_BINARY_DIR}/spatialroot-realtime-build"
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/BuildConfig.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/BuildConfig.hpp"
    @ONLY
)

add_executable(alloRoot
    src/main.cpp
    src/AlloRootApp.cpp
    src/SpatialRootSessionWrapper.cpp
)

target_include_directories(alloRoot PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

target_link_libraries(alloRoot PRIVATE
    al
    EngineSessionCore
)

if(APPLE)
    target_compile_definitions(alloRoot PRIVATE GL_SILENCE_DEPRECATION)
endif()
