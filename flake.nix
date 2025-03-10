{
  description = "hxr a simple tool for hex & binary learning purposes";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable"; 
  };

  outputs = { self, nixpkgs }: 
  let 
    system = "x86_64-linux"; 
    pkgs = import nixpkgs { inherit system; };
  in {
    
    packages.${system}.default = pkgs.stdenv.mkDerivation {
      pname = "hxr";
      version = "1.0";

      src = pkgs.fetchgit {
        url = "https://github.com/6367f766/hxr.git"; 
        rev = "refs/tags/v0.1-alpha";
        sha256 = "9IjwH9/Nu4r/1G0ljG1SRzSo5/OIsTJSKY6zECfvymg=";
        fetchSubmodules = true;
      };

      buildInputs = with pkgs; [
        git
        cmake
        gtest # XXX: This is not needed for release builds... but we don't have release builds yet...
      ]; 

      buildPhase = ''
        cd ..
        cmake --workflow --preset release --fresh
      '';

      installPhase = ''
        mkdir -p $out/bin
        cp build_outputs/release/main/main $out/bin/hxr
        chmod +x $out/bin/hxr
      '';
    };

    apps.${system}.default = {
      type = "app";
      program = "${self.packages.${system}.default}/bin/hxr";
    };

    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [ 
        cmake 
        gtest
        gdb
        clang-tools
        doxygen
        graphviz
      ];

      IS_NIX_SHELL = 1;
    };
  };
}
