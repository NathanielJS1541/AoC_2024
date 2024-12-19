let
  pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
    name = "day17";
    nativeBuildInputs = with pkgs.buildPackages; [
        just
        lazygit
        neovim
        clojure
        babashka
    ];
    shellHook = ''
        echo ""
        echo "Advent of Code 2024"
        echo "Day 17: Chronospatial Computer - https://adventofcode.com/2024/day/17"
        echo ""
    '';
}
