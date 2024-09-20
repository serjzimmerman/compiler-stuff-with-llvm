{
  rustPlatform,
  lib,
  SDL2,
  llvmPackages_18,
  meson,
  ninja,
  pkg-config,
  cmake,
}:

rustPlatform.buildRustPackage {
  pname = "compiler-stuff-with-llvm";
  version = "0.1.0";

  src = lib.cleanSource ./.;
  cargoLock = {
    lockFile = ./Cargo.lock;
  };

  nativeBuildInputs = [
    llvmPackages_18.libllvm
    meson
    ninja
    pkg-config
    cmake
  ];

  buildInputs = [ SDL2 ];
}
