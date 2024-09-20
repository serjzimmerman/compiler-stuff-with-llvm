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
      ];

      perSystem =
        {
          pkgs,
          ...
        }:
        rec {
          packages =
            let
              rustToolchain = pkgs.rust-bin.fromRustupToolchainFile ./rust-toolchain.toml;
              rustPlatform = pkgs.makeRustPlatform {
                cargo = rustToolchain;
                rustc = rustToolchain;
                stdenv = pkgs.llvmPackages_18.stdenv;
              };
            in
            {
              compiler-stuff-with-llvm = pkgs.callPackage ./. {
                inherit rustPlatform;
              };
              default = packages.compiler-stuff-with-llvm;
              inherit rustToolchain;
            };
        };
    };
}
