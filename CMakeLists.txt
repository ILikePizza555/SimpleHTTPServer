cmake_minimum_required(VERSION 3.10)

project(SimpleHTTPServer)

# Define the sources for SimpleHTTPServer
set(HTTP_SOURCES src/http/Http.cpp src/http/HttpServer.cpp src/http/HttpUtils.cpp src/http/HttpError.cpp)

add_executable(shs src/main.cpp src/string_utils.cpp ${HTTP_SOURCES})

# Include sockets.cpp into the project
set(SOCKETSCPP_VERSION "v1.0.1")
set(SOCKETSCPP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/sockets.cpp-${SOCKETSCPP_VERSION})
include(ExternalProject)
ExternalProject_Add(socketscpp
        GIT_REPOSITORY  git@github.com:ILikePizza555/Sockets.cpp.git
        GIT_TAG         ${SOCKETSCPP_VERSION}
        PREFIX          ${SOCKETSCPP_PREFIX}
        CMAKE_ARGS      "-DCMAKE_INSTALL_PREFIX=${SOCKETSCPP_PREFIX}")

ExternalProject_Get_Property(socketscpp install_dir)
include_directories(${install_dir}/include)

add_dependencies(shs socketscpp)
target_link_libraries(shs ${install_dir}/lib/libsocketscpp.a)