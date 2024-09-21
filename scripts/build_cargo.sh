#!/usr/bin/env bash

set -x

root_repo_path="$(dirname "$0")/.."
cd "${root_repo_path}" || exit
cargo build "${@:3}"
cp target/"$1"/libcompiler_stuff_with_llvm.a "$2"
