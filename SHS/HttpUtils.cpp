#include "HttpUtils.h"

//TODO: Make this into some file parser or something
std::string Http::defaultHtml(std::string title, std::string header, std::string message)
{
	return utils::string::concat({"<html><head><title>",
		title,
		"</title></head><body><h1>",
		header,
		"</h1><p>",
		message,
		"</p><hr/><sup><i>",
		SERVER_NAME,
		"</i></sup></body></html>"});
}

Http::HttpResponse Http::buildError(int statusCode, std::string reason, std::string html)
{
	HttpResponse rv{
		HTTP_VERSION,
		statusCode,
		reason,
		{
			{"Content-Type", CT_HTML},
			{"Content-Length", std::to_string(html.length())},
			{"Server", SERVER_NAME},
			{"Connection", "close"}
		},
		html
	};
	return rv;
}

std::string Http::guessMime(std::string filename)
{
	std::string ext = utils::string::split(filename, ".").back();

	if (utils::string::beginsWith(ext, "htm")) return "text/html";
	if (ext == "bmp") return "image/bmp";
	if (ext == "png") return "image/png";
	if (ext == "jpeg" || ext == "jpg") return "image/jpeg";
	if (ext == "gif") return "image/gif";
	if (ext == "json") return "application/json";
	if (ext == "css") return "text/css";

	return "application/octet-stream";
}

void Http::sendResponse(sockets::ClientConnection& client, HttpResponse& res)
{
	client.buffer.clear();
	auto data = serializeHttpResponse(res);
	int ptr = 0;

	//If the data is small enough, just send it
	if (data.length() <= client.buffer.getLength())
	{
		client.buffer.assign(data);
		client.send();
		return;
	}

	//Otherwise, we have to chunk it
	while (ptr < data.length())
	{
		client.buffer.assign(data.substr(ptr, client.buffer.getLength()));
		client.send();

		client.buffer.clear();
		ptr += client.buffer.getLength();
	}
}

std::ifstream Http::openFile(HttpRequest& req, std::string rootFile)
{
	//Modify the req if it's root
	if (req.path.back() == '/') req.path.append(rootFile);
	return std::ifstream(req.path.substr(1, req.path.length()), std::ios::binary);
}
