# uvclientserver

An client/server example using libuv

## Requirements

- C++17 compiller. It was tested on Apple clang version 12.0.0
- Any OS. It was tested on macOS

## Compilation

```bash
git clone https://github.com/ivan-volnov/uvclientserver.git
cd uvclientserver
mkdir build
cd build/
cmake ..
make
```

## Usage

Server usage:

```bash
./uv_client host port source_file [bandwidth]
```

Bandwidth param is optional and must be set in MBit/s

Client usage:

```bash
./uv_server host port
```

Due to security reasons the file will be stored
in server current working directory
without respecting of full file path

To start unit tests:

```bash
./test/test_uvclientserver
```
