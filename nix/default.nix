{
  stdenv,
  pkg-config,
  meson,
  ninja,
  lib,
  gtkmm4,
  cmake,
  gtk4-layer-shell,
  wrapGAppsHook4,
}:
stdenv.mkDerivation {
  pname = "lumastart";
  version = "0.0.2";

  src = builtins.path {
    name = "lumastart-source";
    path = ../.;
  };

  nativeBuildInputs = [
    pkg-config
    meson
    ninja
    cmake
    # allows to get the icon from the theme on nix
    wrapGAppsHook4
  ];

  buildInputs = [
    gtk4-layer-shell
    gtkmm4
  ];

  meta = {
    description = "gtk system launcher";
    homepage = "https://github.com/spector700/lumastart";
    license = lib.licenses.gpl3;
    # maintainers = with lib.maintainers; [ ];
  };
}
