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
	(request.size() == 3) ? rv.version = request[2] : rv.version = "HTTP/1.1";

	//Parse the headers
	int header_end = std::find_if(lines.begin() + 1, lines.end(), [](std::string& s) {return s.empty();}) - lines.begin();
	for(int i = 1; i < header_end; i++) {
		auto line = utils::split(lines[i], ":", 1);
		utils::trim(line[0]);
		utils::trim(line[1]);
		rv.headers.insert(std::make_pair(line[0], line[1]));
	}

	rv.body = utils::concat(lines, "", header_end);

	return rv;
}