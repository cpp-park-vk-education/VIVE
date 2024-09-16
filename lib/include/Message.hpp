#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <sstream>

class Message
{
public:
    static constexpr size_t header_length = 4;
    static constexpr size_t max_body_length = 512;

private:
    char data[header_length + max_body_length];
    size_t body_length;
    int request_type;

public:
    Message();
    Message(const std::string& mess);
    
    int getRequestType() const;
    char* getDataToWrite();
    const char* getData() const;
    char* getBodyToWrite();
    const char* getBody() const;
    size_t getBodyLength() const;
    void decodeHeader();
};
