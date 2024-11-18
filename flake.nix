{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

    flake-parts = {
      url = "github:hercules-ci/flake-parts";
      inputs.nixpkgs-lib.follows = "nixpkgs";
    };

    rust-overlay.url = "github:oxalica/rust-overlay";

    treefmt-nix = {
      url = "github:numtide/treefmt-nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      flake-parts,
      ...
    }@inputs:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      imports = [
        ./nix/rust-overlay/flake-module.nix
        ./nix/treefmt/flake-module.nix
        ./nix/devshell/flake-module.nix
      ];

      perSystem =
        {
          pkgs,
          lib,
          self',
          ...
        }:
        {
          packages =
            let
              # https://www.github.com/oxalica/rust-overlay/issues/70#issuecomment-1153789106
              rustToolchain = (pkgs.rust-bin.fromRustupToolchainFile ./rust-toolchain.toml).overrideAttrs {
                propagatedBuildInputs = [ ];
                depsHostHostPropagated = [ ];
                depsTargetTargetPropagated = [ ];
              };
              rustPlatform = pkgs.makeRustPlatform {
                cargo = rustToolchain;
                rustc = rustToolchain;
              };
            in
            rec {
              cswl-sim = pkgs.callPackage ./cswl-sim {
                inherit rustPlatform;
              };
              default = pkgs.callPackage ./. {
                inherit rustToolchain rustPlatform;
                llvmPackages = pkgs.llvmPackages_18;
              };
              cswl-everything = default;
              cswl-everything-static = default.overrideAttrs (prevAttrs: {
                mesonFlags = prevAttrs.mesonFlags ++ [
                  (lib.mesonOption "default_library" "static")
                ];
              });
              cswl-everything-llvm = default.overrideAttrs (prevAttrs: {
                mesonFlags = prevAttrs.mesonFlags ++ [
                  (lib.mesonOption "compiler_backend" "llvm")
                ];
              });
              inherit rustToolchain;
            };

          checks = {
            inherit (self'.packages)
              cswl-sim
              cswl-everything
              cswl-everything-static
              cswl-everything-llvm
              ;
          };
        };
    };
}
