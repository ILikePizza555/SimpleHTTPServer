#pragma once

#include <exception>
#include <string>

namespace Http
{
	class HttpError : public std::exception
	{
		std::string message;

	public:
		explicit HttpError(const std::string& message);

		const char* what() const noexcept override;
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
