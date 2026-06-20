#!/usr/bin/env bash
set -euo pipefail
# Build vanilux inside Ubuntu 22.04 Docker container
# to target glibc 2.35 (compatible with Mint 21.x / Ubuntu 22.04+)

APP="vanilux"
SRC_DIR="$(cd "$(dirname "$0")" && pwd)"

docker run --rm -i \
  -v "$SRC_DIR:/src:ro" \
  -v "$SRC_DIR:/out" \
  ubuntu:22.04 \
  bash -c '
set -euxo pipefail
export DEBIAN_FRONTEND=noninteractive

# Install build deps
apt-get update -qq
apt-get install -y -qq build-essential make pkg-config \
  libgtkmm-3.0-dev 2>/dev/null

# Build
cd /src
VERSION="$(cat VERSION)"
make clean 2>/dev/null || true
make CXXFLAGS="-O2 -g -std=c++17 -fno-strict-aliasing -Wall -Wextra -DVLX_VERSION='\"'$VERSION'\"'"

# Check glibc requirement
echo "=== GLIBC requirements ==="
objdump -T vanilux | grep -oP "GLIBC_\S+" | sort -Vu
echo "=== Minimum GLIBC version needed ==="
objdump -T vanilux | grep -oP "GLIBC_\S+" | sed "s/GLIBC_//" | sort -V | tail -1

# Copy back
cp vanilux /out/vanilux-docker
chmod 755 /out/vanilux-docker
echo "=== BUILD DONE ==="
'

# Verify the docker-built binary
echo "=== Verifying docker binary ==="
ldd "$SRC_DIR/vanilux-docker" | grep -i "not found" || echo "All libs resolved"
objdump -T "$SRC_DIR/vanilux-docker" | grep -oP "GLIBC_\S+" | sort -Vu
echo "=== Minimum GLIBC ==="
objdump -T "$SRC_DIR/vanilux-docker" | grep -oP "GLIBC_\S+" | sed "s/GLIBC_//" | sort -V | tail -1
