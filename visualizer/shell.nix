{ nixpkgs ? import <nixpkgs> {}, compiler ? "default" }:

with nixpkgs;

pkgs.callPackage ./default.nix {}
