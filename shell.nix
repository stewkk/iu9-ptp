{ pkgs ? import <nixpkgs> {} }:

let
  tex = (pkgs.texlive.combine {
    inherit (pkgs.texlive) scheme-full
      latexmk;
  });
in
pkgs.mkShell.override {stdenv = pkgs.llvmPackages_21.stdenv;} {
  buildInputs = [
    pkgs.pandoc
    tex
    pkgs.cmake
    pkgs.pre-commit
    pkgs.plantuml
    pkgs.graphviz
    pkgs.gdb

    # keep this line if you use bash
    pkgs.bashInteractive
  ];

  nativeBuildInputs = [
    pkgs.clang-tools_19
  ];

  NIX_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
    pkgs.stdenv.cc.cc
    pkgs.zlib
  ];
  NIX_LD = pkgs.lib.fileContents "${pkgs.stdenv.cc}/nix-support/dynamic-linker";
}
