{
  SDL2,
  cmake,
  fetchFromGitHub,
  fmt_11,
  gtest,
  lib,
  libffi,
  libxml2,
  meson,
  ninja,
  rustPlatform,
  rustToolchain,
  flex,
  bison,
  llvmPackages,
  bash,
  lit,
}:

let
  ctti = fetchFromGitHub {
    owner = "Manu343726";
    repo = "ctti";
    rev = "d7e9828b82ce7a6321465fbd84f9bccb772c7f43";
    hash = "sha256-oeHlK5M6lHgW1hVCE9UO6Z1DHO2raf0fGeAkSYN0Spw=";
  };

  poplVersion = "1.3.0";

  popl = fetchFromGitHub {
    owner = "badaix";
    repo = "popl";
    rev = "v${poplVersion}";
    hash = "sha256-AkqFRPK0tVdalL+iyMou0LIUkPkFnYYdSqwEbFbgzqI=";
  };
in

llvmPackages.stdenv.mkDerivation (finalAttrs: {
  pname = "cswl-everything";
  version = "0.1.0";

  src = lib.cleanSource ./.;

  cargoDeps = rustPlatform.fetchCargoTarball {
    inherit (finalAttrs) src;
    hash = "sha256-xMHZuLlNemZKYYtHGZdIQ7+7BL/wkiIJJOn4dTEWYmI=";
  };

  nativeBuildInputs = [
    cmake
    meson
    ninja
    rustPlatform.cargoSetupHook
    rustToolchain
    flex
    bison
    lit
    llvmPackages.llvm
  ];

  buildInputs = [
    SDL2
    libffi
    libxml2
    fmt_11
  ];

  dontUseCmakeConfigure = true;

  prePatch =
    # TODO: Get patchShebangs working and get rid of substituteInPlace.
    let
      poplDir = "popl-${poplVersion}";
    in
    ''
      cp -r ${ctti} subprojects/ctti
      cp -r ${popl} subprojects/${poplDir}
      chmod 0755 subprojects/${poplDir}
      cp subprojects/packagefiles/${poplDir}/meson.build subprojects/${poplDir}
      substituteInPlace cswl-sim/scripts/build_cargo.sh \
        --replace-fail '/usr/bin/env bash' '${lib.getExe bash}'
    '';

  doCheck = true;

  mesonFlags = [
    (lib.mesonBool "tests" finalAttrs.doCheck)
  ];

  checkInputs = [
    gtest
  ];
})
