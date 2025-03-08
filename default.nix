{ pkgs ? import <unstable> { } }:
 
pkgs.mkShell ({
    name = "env";
    buildInputs = with pkgs; [
        cmake 
        gtest
        gdb
        clang-tools
        doxygen
    ];

    IS_NIX_SHELL = 1;
})
