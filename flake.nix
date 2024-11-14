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
              };
              cswl-everything = default;
              inherit rustToolchain;
            };

          checks = {
            inherit (self'.packages) cswl-sim cswl-everything;
          };
        };
    };
}
