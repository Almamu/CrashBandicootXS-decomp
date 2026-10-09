This project assumes you already are familiar with a Linux Terminal.

The ROM is built entirely from this repository's sources, so you don't
need a copy of the game to build it. A copy of Crash Bandicoot XS
(Europe) named `baserom.gba` in the project's root is optional: the
progress report's data units use it for their target bytes when it's
there, and a few one-off extraction tools in `tools/` read it.

# Build pre-requirements
## Windows (WSL)/Debian/Ubuntu
### Packages
You'll need to install basic build tools first. These are needed to build the agbcc toolchain and the final rom.

```sh
sudo apt update
sudo apt install build-essential binutils-arm-none-eabi gcc-arm-none-eabi libpng-dev zlib1g-dev python3-pil
```

## MacOS
### Packages
You'll need to install basic build tools first. These are needed to build the agbcc toolchain and the final rom.
```sh
xcode-select --install
brew install arm-none-eabi-binutils arm-none-eabi-gcc arm-none-eabi-gdb libpng zlib
pip3 install Pillow
```

### Toolchain
Once those packages are installed you'll need to provide agbcc, which is the compiler used for building the final ROM.

1. Clone [agbcc](https://github.com/SAT-R/agbcc) into any folder
2. Build agbcc using the `./build.sh` script.
3. Install the compiler under the project's directory using the install script `./install.sh path/to/decomp/project`.
   This installs `tools/agbcc/bin/agbcc` plus the two other builds the ROM needs: `old_agbcc` and the ARM-targeting `agbcc_arm`.
4. Build the C++ compilers from the `cp` branch of [notyourav/agbcc](https://github.com/notyourav/agbcc) (the g++ 2.9 front end, decomp.me's "agbccpp"). From the decomp project's directory:
   ```
   git clone -b cp https://github.com/notyourav/agbcc path/to/agbcc-cp
   tools/build_agbccpp.sh path/to/agbcc-cp
   ```
   This copies the checkout, applies `tools/agbcc_patches/agbcp_agbcc_options.patch` (agbcc's `-fprologue-bugfix` and old-compiler switches) and installs `tools/agbcc/bin/agbcp` and `tools/agbcc/bin/old_agbcp`. The game objects written as C++ (`src/*/*.cpp`) need them; see [docs/cplusplus.md](docs/cplusplus.md).
   It also builds the branch's ARM C++ compiler (`g++_arm`) with `tools/agbcc_patches/agbcc_arm_prologue_return.patch` applied and installs it as `tools/agbcc/bin/agbcp_arm_patched`, the compiler of the IWRAM image's ARM code (`src/iwram/*_arm.cpp`). Four of its functions need the patch: the ROM's ARM compiler was a later build than agbcc_arm, and the patch adds four opt-in options that reproduce its prologue and return code and two of its jump-pass rules (`-mleaf-no-lr-save`, `-minterwork-return-lr`, `-mno-cond-return`, `-mstrict-cross-jump`; see [docs/matching/iwram-image.md](docs/matching/iwram-image.md), "Seventh pass" and "Ninth step"). A checkout that already has the patches applied works too.
5. Build the ROM with `make compare`
6. A crashbandicootxs.gba file should be created and the message `crashbandicootxs.gba: OK` should appear

`tools/gbagfx` and `tools/grit`, the graphics converters, are built by the Makefile on first use.

For the progress report (`make NON_MATCHING=1 report`, see [docs/decomp_dev.md](docs/decomp_dev.md)), you also need [objdiff-cli](https://github.com/encounter/objdiff/releases).
