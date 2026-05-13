#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

SCENE_PATH="${ALLOWROOT_EXAMPLE_SCENE:-${ROOT_DIR}/build/test-package/scene.lusid.json}"
LAYOUT_PATH="${ALLOWROOT_EXAMPLE_LAYOUT:-${ROOT_DIR}/build/test-layouts/stereo_zero_based.json}"
SOURCES_PATH="${ALLOWROOT_EXAMPLE_SOURCES:-${ROOT_DIR}/build/test-package}"
RUN_SECONDS="${ALLOWROOT_EXAMPLE_RUN_SECONDS:-1}"

if [ ! -x "${ROOT_DIR}/build/alloRoot" ]; then
    echo "alloRoot executable not found."
    echo "Build the project first:"
    echo "  ./build.sh"
    exit 1
fi

if [ ! -f "${SCENE_PATH}" ] || [ ! -f "${LAYOUT_PATH}" ] || [ ! -d "${SOURCES_PATH}" ]; then
    echo "Example assets are incomplete."
    echo "Expected:"
    echo "  scene:   ${SCENE_PATH}"
    echo "  layout:  ${LAYOUT_PATH}"
    echo "  sources: ${SOURCES_PATH}"
    echo ""
    echo "These scripts target the local smoke-test package used during wrapper verification."
    echo "Override any path if needed:"
    echo "  ALLOWROOT_EXAMPLE_SCENE=/path/to/scene.lusid.json"
    echo "  ALLOWROOT_EXAMPLE_LAYOUT=/path/to/layout.json"
    echo "  ALLOWROOT_EXAMPLE_SOURCES=/path/to/source-package"
    exit 1
fi

exec "${ROOT_DIR}/run.sh" \
    --headless \
    --run-seconds "${RUN_SECONDS}" \
    --scene "${SCENE_PATH}" \
    --layout "${LAYOUT_PATH}" \
    --sources "${SOURCES_PATH}" \
    "$@"
