#pragma once

#include <fstream>
#include <sockets/Connection.h>

#include "Http.h"

namespace Http {
	std::string defaultHtml(std::string title, std::string header, std::string message);

	HttpResponse buildError(int statusCode, std::string reason, std::string html);

	std::string guessMime(std::string filename);

	void sendResponse(sockets::TCPConnection& client, HttpResponse& res);

	/**
	 * \brief Opens a file. Also checks for root of directory.
	 * \param req 
	 * \param rootFile 
	 * \return 
	 */
	std::ifstream openFile(HttpRequest& req, std::string rootFile = "index.html");
}