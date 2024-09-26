#include "Message.hpp"

Message::Message() : body_length(0)
{
}

Message::Message(const std::string& mess) : body_length(mess.size())
{
    std::copy(mess.begin(), mess.end(), data);
}

int Message::getRequestType() const
{
    return request_type;
}

char* Message::getDataToWrite()
{
    return data;
}

const char* Message::getData() const
{
    return data;
}

char* Message::getBodyToWrite()
{
    return data + header_length;
}

const char* Message::getBody() const
{
    return data + header_length;
}

size_t Message::getBodyLength() const
{
    return body_length;
}

void Message::decodeHeader()
{
    std::string header(data, header_length);
    //std::copy(data, data + header_length, header);

    body_length = std::stoi(header.substr(1, 3));
    if (body_length > max_body_length)
        std::runtime_error("Oversized message's body!");

    request_type = std::stoi(header.substr(0, 1));
}