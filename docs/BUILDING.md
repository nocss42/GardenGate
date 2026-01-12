# Building

This assumes you have the repository cloned & cd'd into it already.

Here's instructions to build;

# Windows

### Requirements

Visual studio code with 

```
```
### Building minhook (optional)

```
```

# Linux

### Requirements
Have these packages installed

```
llvm-mingw
mingw-w64-binutils
mingw-w64-crt
mingw-w64-gcc
mingw-w64-headers
mingw-w64-tools
mingw-w64-winpthreads
cmake
extra-cmake-modules
```

### Building minhook (optional)
```
rm -r minhook build-minhook
git clone https://github.com/TsudaKageyu/minhook
cmake -S minhook -B build-minhook \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres
cmake --build build-minhook
```

### Building DLL
```
rm -r build-gg
mkdir build-gg
cd build-gg
sh -c "cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw64.cmake \
  -DCMAKE_BUILD_TYPE=Release"
cmake --build .
```

For more info see [the workflows](/.github/workflows/)
