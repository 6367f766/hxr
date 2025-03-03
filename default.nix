{ pkgs ? import <unstable> { } }:
 
pkgs.mkShell ({
    name = "env";
    buildInputs = with pkgs; [
        cmake 
        gtest
        gdb
        clang-tools
    ];

    IS_NIX_SHELL = 1;
})
