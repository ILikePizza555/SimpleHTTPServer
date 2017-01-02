#include "Sockets.h"
#include "string_utils.h"

#include <stdio.h>
#include <string>

#define BUFFERLEN_DEFAULT 512

int main(int argc, char* argv) {
	sockets::init();

	auto server = sockets::ServerSocket();
	bool stop = false;
	static const std::string http = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nServer: Rainbow Dash/0.1 (20% Cooler) (Windows)\r\nConnection:close\r\n\r\n<h1>404 - Not Found</h1>";

	//Networking loop
	while (!stop) {
		server.listen();
		printf("Listening for connections...\n");

		try {
			sockets::ClientConnection client = server.accept();
			client.buffer.clear();

			while (!client.isClosed()) {
				client.read();
				printf(client.buffer.data());

				//Populate the buffer
				client.buffer.clear();
				client.buffer.assign(http);

				//Send it over
				client.send();
				client.shutdown();
			}
		}
		catch (sockets::SocketException e) {
			printf(e.what());	
		}
	}

	return 0;
}