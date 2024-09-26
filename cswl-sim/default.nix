{
  rustPlatform,
  lib,
  SDL2,
  pkg-config,
}:

rustPlatform.buildRustPackage {
  pname = "cswl-sim";
  version = "0.1.0";

  src = lib.cleanSource ./.;
  cargoLock = {
    lockFile = ./Cargo.lock;
  };

  nativeBuildInputs = [ pkg-config ];
  buildInputs = [ SDL2 ];
}
