#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <exception>
#include <string>
#include <vector>

#define IPv4_ANY "0.0.0.0"
#define IPv6_ANY "::"
#define DEFAULT_PORT "80"
#define DEFAULT_BUFFER_SIZE 1024

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

		ClientConnection(SOCKET& cs);

		public:
		std::vector<char> writeBuffer;
		std::vector<char> readBuffer;

		bool isClosed();

		void send();
		void send(int amount);
		void read(size_t amount = DEFAULT_BUFFER_SIZE);
		void close();
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

		void listen(int backlog = SOMAXCONN);
		ClientConnection accept(int bufferSize = DEFAULT_BUFFER_SIZE);

	};

	WSAData init();
}