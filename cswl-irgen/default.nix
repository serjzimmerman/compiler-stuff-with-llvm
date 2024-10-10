{
  rustPlatform,
  lib,
  llvmPackages,
  libffi,
  libxml2,
}:

rustPlatform.buildRustPackage {
  pname = "cswl-irgen";
  version = "0.1.0";

  src = lib.cleanSource ./.;
  cargoLock = {
    lockFile = ./Cargo.lock;
  };

  buildInputs = [
    llvmPackages
    libffi
    libxml2
  ];
}
