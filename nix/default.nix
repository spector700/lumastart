{
  stdenv,
  pkg-config,
  meson,
  ninja,
  lib,
  gtkmm4,
  cmake,
  gtk4-layer-shell,
  wrapGAppsHook,
}:
stdenv.mkDerivation {
  pname = "lumastart";
  applicationName = "lumastart";
  version = "0.0.3";

  src = builtins.path {
    name = "lumastart-source";
    path = ../.;
  };

  nativeBuildInputs = [
    pkg-config
    meson
    ninja
    cmake
    wrapGAppsHook # allows to get the icon from the theme on nix
  ];

  buildInputs = [
    gtk4-layer-shell
    gtkmm4
  ];

  meta = {
    mainProgram = "lumastart";
    description = "System application launcher";
    homepage = "https://github.com/spector700/lumastart";
    license = lib.licenses.gpl3;
    # maintainers = with lib.maintainers; [ ];
  };
}
