#pragma once

#include "GameCalculator.hpp"

class SessionRoom;
using RoomShPtr = std::shared_ptr<SessionRoom>;

class SessionRoom
{
private:
    std::unordered_set<ConnectionShPtr> participants;
    ConnectionShPtr host;
    GameCalculatorUPtr game_calc;

public:
    SessionRoom(ConnectionShPtr connection);

    void join(const ConnectionShPtr new_participant);
    void leave(const ConnectionShPtr participant_to_leave);
    void deliver(const response::Response& resp);
};
