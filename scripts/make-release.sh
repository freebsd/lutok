#!/bin/sh
#
# Create release artifacts from a release tag.
#
# Example:
# 	./scripts/make-release.sh lutok-0.6.3

readonly REPO=freebsd/lutok

set -eux

if ! gh auth status >/dev/null; then
	echo "${0##*/}: ERROR: please install 'gh' and run 'gh auth login'."
	exit 1
fi

tag=$1

cd "$(dirname "$(dirname "$0")")"

mkdir -p releases
release_root=$(realpath releases)

release_dir="${release_root}/${tag}"
release_artifact="${release_root}/${tag}.tar.gz"
release_artifact_checksum_file="${release_artifact}.sha256"
release_assets="${release_artifact} ${release_artifact_checksum_file}"

rm -Rf "${release_dir}"
mkdir -p "${release_dir}"
git archive "${tag}" | tar xzvf - -C "${release_dir}"
cd "${release_dir}"

# Build with CMake to verify everything works
cmake -B build -S . -DBUILD_TESTING=ON -DBUILD_DOCS=ON
cmake --build build
cmake --build build --target docs

# Create source tarball
cd ..
tar czf "${tag}.tar.gz" "${tag}"

cd "${release_root}"
sha256 "${release_artifact##*/}" > "${release_artifact_checksum_file}"

# shellcheck disable=SC2086
gh release upload --repo "${REPO}" "${tag}" ${release_assets}

# vim: syntax=sh
