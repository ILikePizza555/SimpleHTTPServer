#pragma once

#include <exception>
#include <string>

namespace Http
{
	class HttpError : public std::exception
	{
		std::string message;

	public:
		HttpError(std::string message) : message(message)
		{
		}

		const char* what() const override
		{
			return message.c_str();
		}
	};

	class MethodError : public HttpError
	{
		using HttpError::HttpError;
	};

	class RequestError : public HttpError
	{
		using HttpError::HttpError;
	};
}
