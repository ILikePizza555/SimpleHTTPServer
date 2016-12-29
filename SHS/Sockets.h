#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <exception>
#include <string>

#include "Buffer.h"

#define IPv4_ANY "0.0.0.0"
#define IPv6_ANY "::"
#define DEFAULT_PORT "80"
#define DEFAULT_BUFFER_SIZE 2048

namespace sockets {
	class SocketException : std::exception {
		public:
		std::string message;
		int errorCode;

		SocketException(std::string message, int errorCode) : message(message), errorCode(errorCode) {};

		const char* what() const override {
			return message.c_str();
		}
	};

	class ClientConnection {
		friend class ServerSocket;

		private:
		SOCKET clientSocket;
		bool closed;

		ClientConnection(SOCKET& cs, size_t bufferSize);

		public:
		Buffer buffer;

		bool isClosed();

		//Sends the data in writebuffer 
		void send();
		//Reads data into readbuffer
		void read();
		//Closes the sockets
		void close();
		//Tells the client that the connection is closing, and closes the socket
		void shutdown();
	};

	class ServerSocket {
	private:
		std::string ip;
		std::string port;

		SOCKET listenSocket;

	public:
		ServerSocket(std::string ip = IPv6_ANY, std::string port = DEFAULT_PORT);

		~ServerSocket();

		//Enables listening for connections
		void listen(int backlog = SOMAXCONN);
		//Accepts a connection, and returns an object representing the connection (blocks until a connection is established)
		ClientConnection accept(size_t bufferSize = DEFAULT_BUFFER_SIZE);

	};

	WSAData init();
}