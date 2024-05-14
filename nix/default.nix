{
  stdenv,
  pkg-config,
  meson,
  ninja,
  gtkmm,
  lib,
}:
stdenv.mkDerivation {
  pname = "lumastart";
  version = "0.0.1";

  src = ../.;

  nativeBuildInputs = [
    pkg-config
    meson
    ninja
  ];

  buildInputs = [ gtkmm ];

  meta = {
    description = "gtk system launcher";
    homepage = "https://github.com/spector700/lumastart";
    license = lib.licenses.gpl3;
    maintainers = with lib.maintainers; [ ];
  };
}
