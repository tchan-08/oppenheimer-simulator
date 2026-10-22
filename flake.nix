{
  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    inputs@{ flake-parts, ... }:
    flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [ "x86_64-linux" ];
      perSystem =
        { pkgs, lib, ... }:
        {
          devShells.default = pkgs.mkShell {
            nativeBuildInputs = with pkgs; [
              gnumake
              clang
              pkg-config
              bear
            ];

            buildInputs = with pkgs; [
              vulkan-loader
              libGL
              SDL2
              SDL2_image
              SDL2_mixer
            ];

            env.LD_LIBRARY_PATH = lib.makeLibraryPath (
              with pkgs;
              [
                sdl3
                vulkan-loader
                libGL
              ]
            );
          };
        };
    };
}
