#!/bin/sh
#
# Copyright (c) 2026 Enji Cooper.
#
# SPDX-License-Identifier: BSD-2-Clause

# Step 1: run cmake configuration so the remaining build/test steps can
# be completed.
#
# Splitting off this step allows any configuration issues to be found quickly
# and triaged more effectively.

set -eux

CMAKE_ARGS="-DBUILD_TESTING=ON -DBUILD_DOCS=OFF"

# Developer mode is auto-enabled for git repos, but we can force it
if [ -n "${DEVELOPER_MODE:-}" ]; then
    CMAKE_ARGS="${CMAKE_ARGS} -DDEVELOPER_MODE=${DEVELOPER_MODE}"
fi

if ! cmake -B build -S . ${CMAKE_ARGS}; then
    cat build/CMakeFiles/CMakeError.log || true
    cat build/CMakeFiles/CMakeOutput.log || true
    exit 1
fi

# vim: syntax=sh:expandtab:shiftwidth=4:softtabstop=4
