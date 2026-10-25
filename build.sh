#!/bin/bash
# Build script for Click Buffer mod

set -e

GEODE_SDK="${GEODE_SDK:-$HOME/.geode/sdk}"

if [ ! -d "$GEODE_SDK" ]; then
    echo "Geode SDK not found at $GEODE_SDK"
    echo "Install Geode SDK or set GEODE_SDK environment variable"
    exit 1
fi

echo "Building Click Buffer mod..."
echo "Using Geode SDK: $GEODE_SDK"

mkdir -p build
cd build

cmake -DGEODE_SDK_PATH="$GEODE_SDK" ..
cmake --build . --config Release

echo ""
echo "Build complete!"
echo "Mod file: $(ls -1 *.geode 2>/dev/null || echo 'Check build directory')"
