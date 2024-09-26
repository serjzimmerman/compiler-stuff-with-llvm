#!/usr/bin/env bash

set -x

echo "$2"
root_repo_path="$(dirname "$0")/.."
cd "${root_repo_path}" || exit
CARGO_TARGET_DIR=$(pwd)/target cargo build "${@:3}"
library_name=$(basename "$2")
mkdir "$(dirname "$2")" -p
cp target/"$1"/"$library_name" "$2"
