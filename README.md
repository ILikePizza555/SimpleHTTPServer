## What

As the name suggests, this is a simple HTTP server I wrote using nothing but Winsock (restricting myself to standard POSIX functions) and the STL. It's intended to be cross-platform, and light on CPU usage and memory for modest amounts of traffic. It compiles successfully in Visual Studio 2015

## User Usage

1. Compile it
2. Run it.

## Programmer usage

1. call `sockets::init()`
2. Define an `Http::HttpServer` object
3. Call `start()` on the server object.
4. That's it.

Check the included main.cpp for an example.

You can extend `Http::HttpServer` and override `httpRequestHandler` to implement your own custom functionality. By default the server only responds to GET requests. All other requests return 501 Not Implemented.

## TODO

- Profile function and optimize as needed.
- Convert to makefiles
- Seperate into two projects: a library and application
- Configuration files
- Add more default functionality
- Chunked delivery
- Gzip encoding
