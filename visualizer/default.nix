{ stdenv, lib, autoreconfHook, pkg-config, gtkmm3 }:

stdenv.mkDerivation {
  pname = "visualizer";
  version = "0.2";

  src = lib.cleanSource ../.;
  sourceRoot = "source/visualizer";

  buildInputs = [
    autoreconfHook pkg-config gtkmm3
  ];
}
