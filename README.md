# cipher-cli

CLI utility to encrypt and decrypt files

# Dev

Project uses CMake and Conan

##  Build and run

### On Windows

```bat
conan install . --output-folder=build --build=missing
build/conanbuild.bat
cmake -S . -B build -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake"
cmake --build build --config Release
build/deactivate_conanbuild.bat
build/Release/cipher-cli.exe
```

# Notice

The project uses the following 3rd party libraries
- [`cli11`](https://conan.io/center/recipes/cli11?version=2.4.2)
- [`cryptopp`](https://conan.io/center/recipes/cryptopp?version=8.9.0)
