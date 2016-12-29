#include "Sockets.h"

#include <stdio.h>

#define BUFFERLEN_DEFAULT 512

int main(int argc, char* argv) {
	sockets::init();

	auto server = sockets::ServerSocket();
	bool stop = false;
	static char http[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nServer: Rainbow Dash/0.1 (20% Cooler) (Windows)\r\nConnection:close\r\n\r\n<h1>404 - Not Found</h1>";


	//Networking loop
	while (!stop) {
		server.listen();
		printf("Listening for connections...\n");

		try {
			sockets::ClientConnection client = server.accept();

			while (!client.isClosed()) {
				client.read();
				printf(client.readBuffer.data());

				//Populate the write buffer
				client.writeBuffer.clear();
				client.writeBuffer.assign(http, http + strlen(http));

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