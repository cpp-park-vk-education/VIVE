#pragma once

#include <boost/signals2.hpp>
#include <functional>
#include "SignalManager.hpp"

class RoomSignals;
using RoomSignalsUPtr = std::unique_ptr<RoomSignals>;


class RoomSignals
{
public:
    RoomSignals() = default;

    void connect(SignalManager& room_manager)
    {
        requestNewRoom.connect(std::bind(&SignalManager::getNewRoom, room_manager));
        joinRoom.connect(std::bind(&SignalManager::joinExistentRoom, room_manager, std::placeholders::_1));
        leaveRoom.connect(std::bind(&SignalManager::leaveCurrentRoom, room_manager));
        askToDeliver.connect(std::bind(&SignalManager::deliverToPlayers, room_manager, std::placeholders::_1));
    }

    boost::signals2::signal<int()> requestNewRoom;
    boost::signals2::signal<bool(int)> joinRoom;
    boost::signals2::signal<bool()> leaveRoom;
    boost::signals2::signal<void(Message)> askToDeliver;
};