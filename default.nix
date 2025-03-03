{ pkgs ? import <unstable> { } }:
 
pkgs.mkShell ({
    name = "env";
    buildInputs = with pkgs; [
        cmake 
        gtest
        gdb
    ];

    IS_NIX_SHELL = 1;
})
