# Windows runtime components
For Lang executables, keep libgmp-10.dll beside the .exe. This is GMP 6.3.0-2 from MSYS2 UCRT64. Its matching source, build recipe, and patches are in mingw-w64-gmp-6.3.0-2.src.tar.zst; the COPYING files give the GMP terms.

The SIGFPE Haskell executables use GHC 9.10.3 (x86_64 Windows) with its bundled GMP backend. ghc-9.10.3-relink.zip includes benchmark objects by program, license notices, and GHC's bundled GMP source and patches. To relink, use the corresponding [GHC Windows toolchain](https://downloads.haskell.org/~ghc/9.10.3/) with the native libraries you want to use. For example, from the extracted archive:

    ghc -O2 objects/SeriesComposition/Main.o objects/SeriesComposition/BenchRepeat.o -o series-composition.exe

The [GHC 9.10.3 source archive](https://downloads.haskell.org/~ghc/9.10.3/ghc-9.10.3-src.tar.xz) supplies the toolchain and library build sources (SHA-256 d266864b9e0b7b741abe8c9d6a790d7c01c21cf43a1419839119255878ebc59a).

LLVM-compiler-rt-LICENSE.txt contains the LLVM compiler runtime notices.
