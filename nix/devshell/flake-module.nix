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
                libxml2
                gtest
                fmt_11
              ])
              ++ [ llvmPackages.libllvm ];
            nativeBuildInputs = with pkgs; [
              ninja
              meson
              pkg-config
              cargo-watch
              cmake
              flex
              bison
            ];
          }).overrideAttrs
            { allowSubstitutes = false; };
      };
    };
}
