# Building
## Windows

### DLL

```sh
cd DLL
cmake . -B build -G "Visual Studio 18 2026"
cmake --build build --config Release --parallel %NUMBER_OF_PROCESSORS%
```

### Launcher

```sh
cd Launcher
cmake . -B build -G "Visual Studio 18 2026"
cmake --build build --config Release --parallel %NUMBER_OF_PROCESSORS%
```

## Linux
### DLL

```sh
cd DLL
export WINDOWS_SDK_PATH="$HOME/my_msvc/opt/msvc"
mkdir -p $WINDOWS_SDK_PATH
git clone https://github.com/mstorsjo/msvc-wine
./msvc-wine/vsdownload.py --accept-license --dest $WINDOWS_SDK_PATH
./msvc-wine/install.sh $WINDOWS_SDK_PATH
export PATH=$WINDOWS_SDK_PATH/bin/x64:$PATH
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DWINDOWS_SDK_PATH=$WINDOWS_SDK_PATH -DCMAKE_TOOLCHAIN_FILE=./toolchain-linux-winsdk.cmake
make -C build -j$(nproc)
```

### Launcher

```sh
cd Launcher
cmake . -B build -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw64.cmake
cmake --build build --config Release --parallel "$(nproc)"
```

See [the workflows](../.github/workflows/)
