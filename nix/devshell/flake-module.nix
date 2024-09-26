{
  perSystem =
    { self', pkgs, ... }:
    let
      llvmPackages = pkgs.llvmPackages_18;
      mkShellWithLlvm = (
        pkgs.mkShell.override {
          stdenv = llvmPackages.stdenv;
        }
      );
    in
    {
      devShells = {
        default =
          (mkShellWithLlvm {
            name = "llvm-shell-for-cswl";

            buildInputs = with pkgs; [
              SDL2
              self'.packages.rustToolchain
            ];
            nativeBuildInputs = with pkgs; [
              ninja
              meson
              pkg-config
            ];
          }).overrideAttrs
            { allowSubstitutes = false; };
      };
    };
}
