{ inputs, ... }:

let
  overlays = [
    (import inputs.rust-overlay)
  ];
in

{
  perSystem =
    { system, ... }:
    {
      _module.args.pkgs = import inputs.nixpkgs {
        inherit system overlays;
        config = { };
      };
    };
}
