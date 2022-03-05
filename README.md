# Temp
Temporary git archive

## about Temp

Currently testing the following wepage codes
https://www.kabuku.co.jp/developers/webassembly

For original gol.wat:
```console
$ sudo apt install git
$ sudo apt install build-essential
$ sudo apt install cmake
$ git clone --recursive https://github.com/WebAssembly/wabt -b 1.0.24
$ cd wabt
$ git submodule update --init
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./wat2wasm gol.wat
$ sha1sum gol.wasm
61ec7d19334b97c0c2fb3e7d901a5e96fd480726  gol.wasm
```
For modified gol.wat (where "set_local" and "get_local" were replaced by "local.set" and "local.get", respectively:
```console
$ sudo apt install git
$ sudo apt install build-essential
$ sudo apt install cmake
$ git clone --recursive https://github.com/WebAssembly/wabt -b 1.0.27
$ cd wabt
$ git submodule update --init
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ ./wat2wasm gol.wat
$ sha1sum gol.wasm
61ec7d19334b97c0c2fb3e7d901a5e96fd480726  gol.wasm
```
