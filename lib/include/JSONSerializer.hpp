#pragma once

#include "ISerializable.hpp"

class JSONSerializer: public ISerializable
{
public:
    std::string serialize(const Message& mess) override;
    Message deserialize(const std::string& ser_string) override;
};