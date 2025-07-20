#!/usr/bin/env bash
set -e

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  sudo apt-get update
  sudo apt-get install -y gcc-14 clang-16
  echo "GCC 14 and Clang 16 installed."
elif [[ "$OS" == "Windows_NT" ]]; then
  echo "Please install MSVC 19.38 via Visual Studio Installer." >&2
else
  echo "Unsupported OS" >&2
  exit 1
fi
