#include "HttpError.h"

namespace Http
{
    HttpError::HttpError(const std::string& message): message(message) {}

    const char* HttpError::what() const noexcept
    {
        return message.c_str();
    }
}