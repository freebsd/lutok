#!/bin/sh
#
# Copyright (c) 2026 Enji Cooper.
#
# SPDX-License-Identifier: BSD-2-Clause

# Step 2: build and test the project.
#
# This builds the library, runs tests with kyua, and optionally installs.

set -eux

: "${CC=cc}"
: "${CXX=c++}"

NPROC=$(nproc 2>/dev/null || getconf NPROCESSORS_ONLN 2>/dev/null || echo 1)

# Build the library
cmake --build build -j"${NPROC}"

# Run tests if ATF is available
if [ -d build ] && cmake --build build --target help 2>&1 | grep -q test; then
    # Check if we have test executables
    if ls build/*_test 2>/dev/null || ls build/tests/*_test 2>/dev/null; then
        kyua_conf="$(realpath "$(mktemp kyuaconf-XXXXXXXX)")"
        trap 'rm -f "${kyua_conf}"' EXIT INT TERM

        cat >"${kyua_conf}" <<EOF
syntax(2)

unprivileged_user = 'nobody'
EOF

        # Run tests with ctest if available, otherwise with kyua
        if command -v ctest >/dev/null 2>&1; then
            (cd build && ctest --output-on-failure -j"${NPROC}")
        fi

        # Also run kyua tests if Kyuafile exists
        if [ -f Kyuafile ]; then
            kyua test --kyuafile=Kyuafile --build-root=build || {
                echo "Tests failed. Kyua results:"
                kyua report --verbose || true
                exit 1
            }
        fi
    else
        echo "No tests found to run (ATF not available)"
    fi
fi

# Optionally test installation
INSTALL_PREFIX="$(pwd)/build/install-test"
cmake --install build --prefix "${INSTALL_PREFIX}"

echo "Build and test completed successfully"

# vim: syntax=sh:expandtab:shiftwidth=4:softtabstop=4
