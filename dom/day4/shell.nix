let
  pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
    name = "day4";
    nativeBuildInputs = with pkgs.buildPackages; [
        just
        lazygit
        neovim
        hy
        python311Packages.hyrule
    ];
    shellHook = ''
        echo ""
        echo "Advent of Code 2024"
        echo "Day 4: Ceres Search - https://adventofcode.com/2024/day/4"
        echo "Hy docs: https://hylang.org/hy/doc/v1.0.0"
        echo ""
    '';
}
