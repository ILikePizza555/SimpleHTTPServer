#include "Sockets.h"

namespace sockets {
	WSAData init() {
		WSADATA wsaData;

		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) throw SocketException("Error on WSAStartup. Error: ", iResult);

		return wsaData;
	}
}