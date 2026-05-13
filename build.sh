#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

if [ ! -f "${SCRIPT_DIR}/thirdparty/spatialroot/CMakeLists.txt" ] && [ -z "${SPATIAL_ROOT_DIR:-}" ]; then
    echo "Spatial Root checkout not found."
    echo "Run:"
    echo "  ./init.sh"
    echo "or set:"
    echo "  SPATIAL_ROOT_DIR=/path/to/spatialroot"
    exit 1
fi

CMAKE_ARGS=(
    -S "${SCRIPT_DIR}"
    -B "${BUILD_DIR}"
)

if [ -n "${SPATIAL_ROOT_DIR:-}" ]; then
    CMAKE_ARGS+=("-DSPATIAL_ROOT_DIR=${SPATIAL_ROOT_DIR}")
fi

echo "Configuring alloRoot..."
cmake "${CMAKE_ARGS[@]}"

echo ""
echo "Building alloRoot..."
cmake --build "${BUILD_DIR}" --parallel

echo ""
echo "Build complete:"
echo "  ${BUILD_DIR}/alloRoot"

