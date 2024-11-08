{pkgs ? import <nixpkgs> {}}:
pkgs.mkShell {
  allowUnfree = true;
  name = "FLSC Portfolio c:";
  packages = with pkgs; [
    clang-tools
    bash-language-server
    valgrind
    alejandra
    cbfmt
    mdformat
    beautysh
    libmicrohttpd
    uncrustify
    shfmt
    gdb
    pnpm
    file
    cmake
  ];
  shellHook = ''
    export PATH="./frontend/node_modules/.bin/:$PATH"
  '';
}
