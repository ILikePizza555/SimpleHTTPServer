## What

As the name suggests, this is a simple HTTP server I wrote using nothing but Winsock (restricting myself to standard POSIX functions) and the STL. It's intended to be cross-platform, and light on CPU usage and memory for modest amounts of traffic. It compiles successfully in Visual Studio 2015

## Usage

Right now, simply compile it and run it! Changing settings requires some small modifications to the source code. (Just a couple of #defines, nothing too big)

You can extend `Http::HttpServer` and override `httpRequestHandler` to implement your own custom functionality. By default the server only responds to GET requests. All other requests return 501 Not Implemented.

## TODO

- Profile functions and optimize as needed.
- Maybe convert to CMake
- Maybe seperate into two projects: a library and application
- Configuration files
- Chunked delivery
- Gzip encoding
- Unit Tests
