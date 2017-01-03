#include "Sockets.h"
#include "Http.h"

#include <stdio.h>
#include <string>

#define BUFFERLEN_DEFAULT 512

int main(int argc, char* argv) {
	sockets::init();

	auto server = sockets::ServerSocket();
	bool stop = false;

	const Http::HttpResponse resp {
		"HTTP/1.1",
		404,
		"Not Found",
		{
			{"Content-Type", "text/html" },
			{"Server", SERVER_NAME }, 
			{"Connection", "close"}
		},
		"<h1>404 - Not Found</h1>"
	 };

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
				Http::HttpRequest req = Http::parseHttpRequest(client.buffer.data());

				//Populate the buffer
				client.buffer.clear();
				client.buffer.assign(Http::buildHttpResponse(resp));

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