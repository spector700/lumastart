{
  # Flake inputs
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  # Flake outputs
  outputs =
    { nixpkgs, ... }:
    let
      # Systems supported
      allSystems = [
        "x86_64-linux" # 64-bit Intel/AMD Linux
        "aarch64-linux" # 64-bit ARM Linux
        "x86_64-darwin" # 64-bit Intel macOS
        "aarch64-darwin" # 64-bit ARM macOS
      ];

      # Helper to provide system-specific attributes
      forAllSystems =
        f: nixpkgs.lib.genAttrs allSystems (system: f { pkgs = import nixpkgs { inherit system; }; });
    in
    {
      # Development environment output
      devShells = forAllSystems (
        { pkgs }:
        {
          default = pkgs.mkShell {
            # The Nix packages provided in the environment
            packages = with pkgs; [
              # Build tools
              pkg-config
              meson
              ninja

              # Libraries
              gtk4-layer-shell
              gtkmm4
              glibmm_2_68
            ];

            # Remove the warning
            hardeningDisable = [ "fortify" ];

            shellHook = ''
              echo "welcome to C++" | ${pkgs.lolcat}/bin/lolcat
            '';
          };
        }
      );

      packages = forAllSystems (
        { pkgs }:
        {
          default = pkgs.callPackage ./nix { };
        }
      );
    };
}
