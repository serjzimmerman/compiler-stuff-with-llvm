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

            buildInputs =
              (with pkgs; [
                SDL2
                self'.packages.rustToolchain
                libffi
              ])
              ++ [ llvmPackages.libllvm ];
            nativeBuildInputs = with pkgs; [
              ninja
              meson
              pkg-config
              cargo-watch
            ];
          }).overrideAttrs
            { allowSubstitutes = false; };
      };
    };
}