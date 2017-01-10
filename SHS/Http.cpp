#include "Http.h"

Http::METHOD Http::getMethod(std::string str) {
	if (str == "GET")			return GET;
	else if (str == "HEAD")		return HEAD;
	else if (str == "POST")		return POST;
	else if (str == "PUT")		return PUT;
	else if (str == "DELETE")	return MDELETE;
	else if (str == "TRACE")	return TRACE;
	else if (str == "OPTIONS")	return OPTIONS;
	else if (str == "CONNECT")	return CONNECT;
	else if (str == "PATCH")	return PATCH;
	else throw MethodError(utils::concat({"Invalid HTTP method: ", str}));
}

Http::HttpRequest Http::parseHttpRequest(std::string httpString) {
	auto lines =  utils::split(httpString, CRLF);
	HttpRequest rv;

	//Parse the request line (method, url, and version)
	auto request = utils::split(lines[0], " ");

	rv.method = getMethod(request[0]);
	rv.path = request[1];
	//Sometimes the version isn't included. That's OK.
	if (request.size() == 3) {
		rv.version = request[2];

		//Need to ensure that the version string is valid
		if (rv.version != "HTTP/1.1" && rv.version != "HTTP/1.0") {
			throw RequestError(utils::concat({ "Supplied version \"", rv.version, "\" is not a valid HTTP version." }));
		}
	} else  {
		rv.version = "HTTP/1.1";
	}

	//Sometimes requests can be only one line. Check to see if we need to parse headers and a body.
	if (lines.size() > 1) {
		//Parse the headers
		int header_end = std::find_if(lines.begin() + 1, lines.end(), [](std::string& s) {return s.empty();}) - lines.begin();
		for(int i = 1; i < header_end; i++) {
			auto line = utils::split(lines[i], ":", 1);
			utils::trim(line[0]);
			utils::trim(line[1]);
			rv.headers.insert(std::make_pair(line[0], line[1]));
		}

		rv.body = utils::concat(lines, "", header_end);
	}

	return rv;
}

std::string Http::serializeHttpResponse(Http::HttpResponse response) {
	std::ostringstream rv;

	//Header line
	rv << response.version << " " << response.statusCode << " " << response.reason << CRLF;

	//HTTP headers
	for (auto& iter : response.headers) rv << iter.first << ":" << iter.second << CRLF;

	//Empty line
	rv << CRLF;

	//Body
	rv << response.body;

	return rv.str();
}

std::string Http::defaultHtml(std::string title, std::string header, std::string message) {
	return utils::concat({  "<html><head><title>", 
							title, 
							"</title></head><body><h1>", 
							header, 
							"</h1><p>",
							message,
							"</p><hr/><sup><i>",
							SERVER_NAME,
							"</i></sup></body></html>"});
}

Http::HttpResponse Http::buildError(int statusCode, std::string reason, std::string html) {
	HttpResponse rv {
		HTTP_VERSION,
		statusCode,
		reason,
		{
			{ "Content-Type", CT_HTML },
			{ "Server", SERVER_NAME },
			{ "Connection", "close" }
		},
		html
	};
	return rv;
}