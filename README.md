A simple, and cross-platform HTTP server written in C++.

## Usage

Simply drop the binary into a directory and run it. SHS will automatically host the files in it's directory on port 80. Currently, all that's supported are simple GET requests. All other requests return 501 Not Implemented.

## Building

### Requirements
All that's needed to build SHS is a C++11 compatible compiler and CMake 3.5 or higher.

This project does depend on my other project [Sockets.cpp](https://github.com/ILikePizza555/Sockets.cpp). However, CMake will automatically download this dependency with the ExternalProject_Add module. Please note that Sockets.cpp requires a C++14 compatible compiler to build. This is because I started work on SHS before I made Sockets.cpp.

### Compiling

Simply run:

```bash
cmake -G [Your generator here]
make
```

## Hacking

If you wish to extend the functionality of SHS, just extend the HTTPServer class and override the httpRequestHandler method. 
