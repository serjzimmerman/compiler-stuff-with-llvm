# 📚 Compiler stuff with LLVM

This repo contains some practice tasks for building LLVM based tooling, passes, e.t.c.

## ❄️ Requirements

To build this repo you need either use [nix](https://nixos.org/) flakes with devshells:
Running the following command will install all of the required dependencies and drop
you in a shell with the right environment:

```bash
nix develop
```

Otherwise you will need the following tools:

- [meson](https://github.com/mesonbuild/meson)
- [ninja](https://github.com/ninja-build/ninja)
- [llvm & clang](https://clang.llvm.org/), default version used by this project is [18](https://releases.llvm.org/18.1.0/docs/ReleaseNotes.html)
- [rust toolchain](https://doc.rust-lang.org/cargo/getting-started/installation.html) built with the same llvm version as the system libraries.
  Working version combination is provided in the project's [flake](./flake.nix) and [rust-toolchain](./rust-toolchain.toml).
- [sdl2](https://github.com/libsdl-org/SDL)
- [libffi](https://github.com/libffi/libffi)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

Steps to install all of those might differ depending on your operating system. If you are not using
the provided flake, then you are on your own.

### Day-Night game and LLVM IR

There are two different implementation of [Day & Night game](<https://en.wikipedia.org/wiki/Day_and_Night_(cellular_automaton)>).
One is written in Rust and the other in C. Both of them need to link with the support crate: [cswl-sim](./cswl-sim). It provides
the [C API](./cswl-sim/include/cswl-sim/bindings.h), which hides all of the nasty SDL2 details behind a small stable binary interface.

To build and run the C version:

```bash
meson setup build -Dbuildtype=release
meson compile -C build day_night_c_ir day_night_c
build/day_night_c
file build/day_night.ll
```

To run the Rust version:

```bash
cargo run --package cswl-sim --release
```

### 🪵 Logging instrumentation pass

Manual build instructions:

```bash
mkdir -p build
cargo build --release
opt --load-pass-plugin target/release/libcswl_pass.so --passes=log-inserter \
  artifacts/01-sdl-stuff/llvm-ir/day_night.ll -o artifacts/02-pass-stuff/llvm-ir/instrumented.ll -S
clang artifacts/02-pass-stuff/llvm-ir/instrumented.ll \
  target/release/libcswl_pass_logger.a \
  target/release/libcswl_sim.a \
  -lSDL2 -O2 -o build/day_night_c_instrumented
```
