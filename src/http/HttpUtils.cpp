#include "HttpUtils.h"

#include "../StringUtils.h"

//TODO: Make this into some file parser or something
std::string Http::defaultHtml(std::string title, std::string header, std::string message)
{
	return utils::concat({"<html><head><title>",
		std::move(title),
		"</title><style>body{margin:2.5em auto;max-width:40.625em;line-height:1.6;font-size:18px;color:#222;padding:0 10px;background-color:#EEE;}</style></head><body><h1>",
		std::move(header),
		"</h1><p>",
		std::move(message),
		"</p><hr/><sup><i>",
		SERVER_NAME,
		"</i></sup></body></html>"});
}

Http::HttpResponse Http::buildError(int statusCode, std::string reason, std::string html)
{
	HttpResponse rv{
		HTTP_VERSION,
		statusCode,
		std::move(reason),
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

std::string Http::guessMime(const std::string& filename)
{
	auto ext = utils::split(filename, ".").back();

	if (utils::beginsWith(ext, "htm")) return "text/html";
	if (ext == "bmp") return "image/bmp";
	if (ext == "png") return "image/png";
	if (ext == "jpeg" || ext == "jpg") return "image/jpeg";
	if (ext == "gif") return "image/gif";
	if (ext == "json") return "application/json";
	if (ext == "css") return "text/css";

	return "application/octet-stream";
}

void Http::sendResponse(sockets::TCPConnection& client, HttpResponse& res)
{
	auto data = serializeHttpResponse(res);
	size_t ptr = 0;

	client.write(data.begin(), data.end());
}

std::ifstream Http::openFile(HttpRequest& req, std::string rootFile)
{
	//Modify the req if it's root
	if (req.path.back() == '/') req.path.append(rootFile);
	return std::ifstream(req.path.substr(1, req.path.length()), std::ios::binary);
}
