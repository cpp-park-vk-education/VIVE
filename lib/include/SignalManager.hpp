#pragma once

#include "Message.hpp"
#include "Request.pb.h"
#include "Response.pb.h"

#include <utility>

class SessionRoom;
using RoomShPtr = std::shared_ptr<SessionRoom>;

class Server;
using ServerShPtr = std::shared_ptr<Server>;

class SignalManager;
using SignalManagerShPtr = std::shared_ptr<SignalManager>;

class Connection;
using ConnectionShPtr = std::shared_ptr<Connection>;

class SignalManager
{
private:
    ServerShPtr server_;
    RoomShPtr room_;
    ConnectionShPtr connection_;

public:
    SignalManager(ServerShPtr server);

    void setConnection(ConnectionShPtr connection);

    int getNewRoom();
    bool joinExistentRoom(int room_id);
    bool leaveCurrentRoom();
    void deliverToPlayers(const Message& mess);
};