#!/usr/bin/env bash
set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  sudo apt-get update
  sudo apt-get install -y gcc-14 clang-16
  echo "GCC 14 and Clang 16 installed."
  # Ensure recent CMake required by the project
  pip install --upgrade --quiet cmake==3.29.0
  echo "CMake 3.29 installed via pip."
elif [[ "$OS" == "Windows_NT" ]]; then
  echo "Please install MSVC 19.38 via Visual Studio Installer." >&2
else
  echo "Unsupported OS" >&2
  exit 1
fi
