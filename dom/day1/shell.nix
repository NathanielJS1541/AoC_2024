let
  pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
    name = "day1";
    nativeBuildInputs = with pkgs.buildPackages; [
        just
        lazygit
        neovim
        hy
	python312Packages.hyrule
    ];
    shellHook = ''
        echo ""
        echo "Advent of Code 2024"
        echo "Day 1: Historian Hysteria - https://adventofcode.com/2024/day/1"
        echo "Hy docs: https://hylang.org/hy/doc/v1.0.0"
        echo ""
    '';
}
