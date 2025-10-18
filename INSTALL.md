This project assumes you already are familiar with a Linux Terminal.

You must have a copy of Crash Bandicoot XS (Europe) named `baserom.gba`.

# Build pre-requirements
## Windows (WSL)/Debian/Ubuntu
### Packages
You'll need to install basic build tools first. These are needed to build the argbcc toolchain and the final rom.

```sh
sudo apt update
sudo apt install build-essential binutils-arm-none-eabi gcc-arm-none-eabi
```

### Toolchain
Once those packages are installed you'll need to provide agbcc, which is the compiler used for building the final ROM.

1. Clone [agbcc](https://github.com/SAT-R/agbcc) into any folder
2. Build argbcc using the `./build.sh` script.
3. Install the compiler under the project's directory using the install script `./install.sh path/to/decomp/project`
4. Copy over the original gameboy advance rom as `baserom.gba` on the root directory of the project
5. Build the ROM with `make`
5. A crashbandicootxs.gba file should be created and a message like `baserom.gba: OK` should appear