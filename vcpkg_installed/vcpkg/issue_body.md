Package: gmp:x64-windows@6.3.0#1

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.42.34433.0
-    vcpkg-tool version: 2024-09-30-ab8988503c7cffabfd440b243a383c0a352a023d
    vcpkg-readonly: true
    vcpkg-scripts version: 2960d7d80e8d09c84ae8abf15c12196c2ca7d39a

**To Reproduce**

`vcpkg install `

**Failure logs**

```
-- Using cached gmp-6.3.0.tar.xz.
-- Cleaning sources at C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/gmp/src/v6.3.0-036e54f1a3.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source C:/Users/xhs/AppData/Local/vcpkg/downloads/gmp-6.3.0.tar.xz
-- Applying patch asmflags.patch
-- Applying patch cross-tools.patch
-- Applying patch subdirs.patch
-- Applying patch msvc_symbol.patch
-- Applying patch arm64-coff.patch
-- Using source at C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/gmp/src/v6.3.0-036e54f1a3.clean
-- Found external ninja('1.12.1').
-- Getting CMake variables for x64-windows
-- Getting CMake variables for x64-windows
-- Using cached autoconf2.71-2.71-3-any.pkg.tar.zst.
-- Using cached msys2-autoconf-wrapper-20221207-1-any.pkg.tar.zst.
-- Using cached msys2-automake-wrapper-20221207-2-any.pkg.tar.zst.
-- Using cached msys2-binutils-2.42-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libtool-2.4.7-3-x86_64.pkg.tar.zst.
-- Using cached msys2-make-4.4.1-2-x86_64.pkg.tar.zst.
-- Using cached msys2-pkgconf-2.1.1-1-x86_64.pkg.tar.zst.
-- Using cached msys2-which-2.21-4-x86_64.pkg.tar.zst.
-- Using cached msys2-bash-5.2.026-1-x86_64.pkg.tar.zst.
-- Using cached msys2-coreutils-8.32-5-x86_64.pkg.tar.zst.
-- Using cached msys2-file-5.45-1-x86_64.pkg.tar.zst.
-- Using cached msys2-gawk-5.3.0-1-x86_64.pkg.tar.zst.
-- Using cached msys2-grep-1~3.0-6-x86_64.pkg.tar.zst.
-- Using cached msys2-gzip-1.13-1-x86_64.pkg.tar.zst.
-- Using cached msys2-diffutils-3.10-1-x86_64.pkg.tar.zst.
-- Using cached msys2-sed-4.9-1-x86_64.pkg.tar.zst.
-- Using cached msys2-msys2-runtime-3.5.3-3-x86_64.pkg.tar.zst.
-- Using cached msys2-automake1.16-1.16.5-1-any.pkg.tar.zst.
-- Using cached msys2-libiconv-1.17-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libintl-0.22.4-1-x86_64.pkg.tar.zst.
-- Using cached msys2-zlib-1.3.1-1-x86_64.pkg.tar.zst.
-- Using cached msys2-tar-1.35-2-x86_64.pkg.tar.zst.
-- Using cached msys2-findutils-4.10.0-2-x86_64.pkg.tar.zst.
-- Using cached msys2-gmp-6.3.0-1-x86_64.pkg.tar.zst.
-- Using cached msys2-gcc-libs-13.3.0-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libbz2-1.0.8-4-x86_64.pkg.tar.zst.
-- Using cached msys2-liblzma-5.6.2-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libzstd-1.5.6-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libreadline-8.2.010-1-x86_64.pkg.tar.zst.
-- Using cached msys2-mpfr-4.2.1-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libpcre-8.45-4-x86_64.pkg.tar.zst.
-- Using cached msys2-perl-5.38.2-2-x86_64.pkg.tar.zst.
-- Using cached msys2-ncurses-6.5-1-x86_64.pkg.tar.zst.
-- Using cached msys2-libxcrypt-4.4.36-1-x86_64.pkg.tar.zst.
-- Using msys root at C:/Users/xhs/AppData/Local/vcpkg/downloads/tools/msys2/40909b65cafccf64
-- Generating configure for x64-windows
CMake Error at scripts/cmake/vcpkg_execute_required_process.cmake:127 (message):
    Command failed: C:/Users/xhs/AppData/Local/vcpkg/downloads/tools/msys2/40909b65cafccf64/usr/bin/bash.exe --noprofile --norc --debug -c "autoreconf -vfi"
    Working Directory: C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/gmp/src/v6.3.0-036e54f1a3.clean/
    Error code: 1
    See logs for more information:
      C:\Users\xhs\source\repos\TerwalEast\TouhouBattleTheatre\vcpkg_installed\vcpkg\blds\gmp\autoconf-x64-windows-err.log

Call Stack (most recent call first):
  scripts/cmake/vcpkg_configure_make.cmake:725 (vcpkg_execute_required_process)
  C:/Users/xhs/AppData/Local/vcpkg/registries/git-trees/0634ea068717e4b97b9a9d767d70179b909657b6/portfile.cmake:86 (vcpkg_configure_make)
  scripts/ports.cmake:192 (include)



```

<details><summary>C:\Users\xhs\source\repos\TerwalEast\TouhouBattleTheatre\vcpkg_installed\vcpkg\blds\gmp\autoconf-x64-windows-err.log</summary>

```
autoreconf-2.71: export WARNINGS=
autoreconf-2.71: Entering directory '.'
autoreconf-2.71: configure.ac: not using Gettext
autoreconf-2.71: running: aclocal --force 
am-wrapper: aclocal: warning: auto-detected versions not found (1.15); falling back to latest available
autom4te-2.71: error: need GNU m4 1.4 or later: /usr/bin/m4
aclocal-1.16: error: /usr/bin/autom4te-2.71 failed with exit status: 1
autoreconf-2.71: error: aclocal failed with exit status: 1
```
</details>

**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "dependencies": [
    "cute-headers",
    {
      "name": "glad",
      "features": [
        "gl-api-46"
      ]
    },
    "glm",
    "opengl",
    "soil2",
    "spdlog",
    "recastnavigation",
    "cgal"
  ],
  "builtin-baseline": "5e5d0e1cd7785623065e77eff011afdeec1a3574"
}

```
</details>
