{
  rustPlatform,
  lib,
  llvmPackages,
  libffi,
}:

rustPlatform.buildRustPackage {
  pname = "cswl-pass";
  version = "0.1.0";

  src = lib.cleanSource ./.;
  cargoLock = {
    lockFile = ./Cargo.lock;
  };

  buildInputs = [
    llvmPackages
    libffi
  ];
}
