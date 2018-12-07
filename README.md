A simple, yet cross-platoform HTTP server written in C++.

## Building

SHS uses CMake for it's build system and depends on my other project: Sockets.cpp.

Configuring with CMake will automatically download the dependency and compile SHS with Sockets.cpp statically linked.
The current build process is:

```bash
cmake -G [Your generator here]
make
```

## Usage

Simply drop the binary into a directory and run it. SHS will automatically host the files in it's directory.

## Roadmap

- Profile functions and optimize as needed.
- Chunked delivery
- Gzip encoding
- Support for other HTTP verbs
