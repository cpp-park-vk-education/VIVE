#pragma once

#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <algorithm> 
#include <cctype>
#include "RoomSignals.hpp"

class Connection;
using ConnectionShPtr = std::shared_ptr<Connection>;

using boost::asio::ip::tcp;

class Connection: public std::enable_shared_from_this<Connection>
{
private:
    static constexpr size_t header_length = 4;

    tcp::socket socket_;
    //Message read_mess;
    request::Request request_;
    std::queue<response::Response> resp_queue_to_write;

    SignalManagerShPtr room_manager_;
    RoomSignalsUPtr room_actions;

    boost::asio::streambuf buf_to_read_in_;

public:
    Connection(tcp::socket socket, SignalManagerShPtr room_manager);
    ~Connection();

    void start();
    void deliver(const response::Response& resp);

private:
    void sendResponse(const response::Response& resp);
    int decodeHeader(std::string& header);
    void doReadHeader();
    void doReadBody(int body_size);
    void doWrite();
    void close();
    void sendSignal();
};