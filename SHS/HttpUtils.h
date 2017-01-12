#pragma once

#include "Http.h"

namespace Http {
	std::string defaultHtml(std::string title, std::string header, std::string message);

	HttpResponse buildError(int statusCode, std::string reason, std::string html);

	std::string guessMime(std::string filename);

	void sendResponse(sockets::ClientConnection& client, HttpResponse& res);
}