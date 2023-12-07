# karac

A tiny C++ wrapper using webview. Acts as a replacement of `webview-nodejs` for kara.

This repository is part of the **Alicorn Again Series**. It's an essential utility for kara.

## Build

You normally won't need to build karac from scratch as it's already included in kara releases. However, if you wish so, please follow these steps.

### Prerequisites

- karac uses webview, so prerequisites from [webview/webview](https://github.com/webview/webview) also applies. (See the C++ part)

- CMake

- On Windows only MinGW-w64 (including MSYS2) is supported. Official builds are compiled using UCRT, but legacy MINGW64 libs should also work. The easiest way to setup is to use [MSYS2](https://www.msys2.org) and install `mingw-w64-ucrt-x86_64-g++` (replace with your arch and `MSYSTEM`) as the compiler.

### Windows Setup

On Windows, Webview2 SDK is required for the build. To do that, get a shell (e.g. MSYS2 or Git Bash) and run `./setup.sh` in the root directory of the project. Please also make sure `wget` and `unzip` are installed and in the `PATH`.

### Build

karac follows the build of a regular CMake project:

```shell
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
ninja
```

You can configure CMake to use your build system as you like. However, only `ninja` is tested for official builds.
