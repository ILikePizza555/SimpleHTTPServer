#pragma once

#include "../string_utils.h"
#include "HttpError.h"

#include <algorithm>
#include <map>
#include <string>
#include <exception>
#include "Sockets.h"

#define SERVER_NAME "SHS/0.9 - Rainbow Dash"
#define CRLF "\r\n"

#define HTTP_VERSION "HTTP/1.1"

#define CT_HTML "text/html"

namespace Http {
	enum METHOD {
		GET,
		HEAD,
		POST,
		PUT,
		MDELETE,
		TRACE,
		OPTIONS,
		CONNECT,
		PATCH
	};

	struct HttpRequest {
		METHOD method;
		std::string path;
		std::string version;

		std::map<std::string, std::string> headers;

		std::string body;
	};

	struct HttpResponse {
		std::string version;
		int statusCode;
		std::string reason;

		std::map<std::string, std::string> headers;

		std::string body;
	};

	METHOD getMethod(std::string str);

	HttpRequest parseHttpRequest(std::string httpString);
	std::string serializeHttpResponse(HttpResponse& res);
}