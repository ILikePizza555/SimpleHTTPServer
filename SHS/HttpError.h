#pragma once

#include <exception>
#include <string>

namespace Http {
	class HttpError : public std::exception {
		const char* message;

	public:
		HttpError(const char* message) : message(message) {}
		HttpError(std::string message) : message(message.c_str()) {}
		const char* what() const override {
			return message;
		}
	};

	class MethodError : public HttpError {
		using HttpError::HttpError;
	};
}