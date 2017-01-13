#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <exception>
#include <string>

#define IPv4_ANY "0.0.0.0"
#define IPv6_ANY "::"
#define DEFAULT_PORT "80"
#define DEFAULT_BUFFER_SIZE 2048

namespace sockets
{
	class SocketException : std::exception
	{
	public:
		std::string message;
		int errorCode;

		SocketException(std::string message, int errorCode) : message(message), errorCode(errorCode)
		{
		};

		const char* what() const override
		{
			return message.c_str();
		}
	};

	class ClientConnection
	{
		friend class ServerSocket;

	protected:
		SOCKET clientSocket;
		sockaddr_storage addr;
		socklen_t len;

		bool closed;

		ClientConnection(SOCKET& cs, sockaddr_storage addr, socklen_t len);

	public:
		bool isClosed() const;
		std::string getIp();

		//Closes the sockets
		void close();
		//Tells the client that the connection is closing, and closes the socket
		void shutdown();
	};

	/**
	 * This class allows you to read from/write to the client, so long as a buffer is provided. 
	 */
	class BufferedClientConnection : public ClientConnection
	{
	private:
		char *buffer;
		size_t length;

	public:
		BufferedClientConnection(ClientConnection c, char* buffer, size_t length);

		//Writes to the buffer
		void assign(const char* string, size_t size) const;
		void assign(std::string data) const;
		//Clears the buffer
		void clear() const;

		//Sends the data into the buffer
		void send() const;
		void send(size_t amount) const;
		//Reads data into the buffer
		void read();
		
		//Returns a pointer to the buffer
		char* data() const;
		size_t getLength() const;

		void operator<< (std::string data) const;
	};

	class ServerSocket
	{
	private:
		std::string ip;
		std::string port;

		SOCKET listenSocket;

	public:
		ServerSocket(std::string ip = IPv6_ANY, std::string port = DEFAULT_PORT);

		~ServerSocket();

		//Enables listening for connections
		void listen(int backlog = SOMAXCONN) const;
		//Accepts a connection, and returns an object representing the connection (blocks until a connection is established)
		ClientConnection accept() const;
	};

	WSAData init();
}
