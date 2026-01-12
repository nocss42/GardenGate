# Building

This assumes you have the repository cloned & cd'd into it already.

Here's instructions to build;

# Windows

### Building minhook (optional)

# Linux (MinGW)

This assumes you have the repository cloned & cd'd into it already.

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

### Building DLL (As of right now you )
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
