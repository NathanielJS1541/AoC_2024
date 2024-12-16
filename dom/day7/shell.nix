let
  pkgs = import <nixpkgs> {};
in
pkgs.mkShell {
    name = "day7";
    nativeBuildInputs = with pkgs.buildPackages; [
        just
        lazygit
        neovim
        luajit
        luajitPackages.luarocks
        luajitPackages.fennel
    ];
    shellHook = ''
        echo ""
        echo "Advent of Code 2024"
        echo "Day 7: Guard Gallivant - https://adventofcode.com/2024/day/7"
        echo ""
    '';
}
