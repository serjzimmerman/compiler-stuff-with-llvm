{ inputs, ... }:

{
  imports = [ inputs.treefmt-nix.flakeModule ];

  perSystem = {
    treefmt = {
      projectRootFile = "flake.nix";
      programs = {
        nixfmt.enable = true;
        yamlfmt.enable = true;
        deadnix.enable = true;
        just.enable = true;
        mdformat.enable = true;
        rustfmt.enable = true;
        clang-format.enable = true;
        meson.enable = true;
      };
    };
  };
}
