#include "JSONSerializer.hpp"

std::string JSONSerializer::serialize(const Message& mess)
{
    return "string message";
}

Message JSONSerializer::deserialize(const std::string& ser_string)
{
    return Message("Message message");
}