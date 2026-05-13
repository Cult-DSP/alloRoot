#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BINARY="${SCRIPT_DIR}/build/alloRoot"

if [ ! -x "${BINARY}" ]; then
    echo "alloRoot executable not found at:"
    echo "  ${BINARY}"
    echo ""
    echo "Build it first:"
    echo "  ./build.sh"
    exit 1
fi

exec "${BINARY}" "$@"

