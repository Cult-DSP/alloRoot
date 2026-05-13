#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Initializing alloRoot submodules..."
git -C "${SCRIPT_DIR}" submodule update --init --recursive

echo ""
echo "Submodules ready."
echo "Next:"
echo "  ./build.sh"

