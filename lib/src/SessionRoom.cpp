#include "SessionRoom.hpp"
#include "JSONSerializer.hpp"

SessionRoom::SessionRoom(ConnectionShPtr connection) : host(connection)
{
    std::cout << "IN ROOM CONNECTION IS INVALID" << (connection == nullptr) << " " << (host == nullptr) << std::endl;
}

void SessionRoom::join(const ConnectionShPtr new_participant)
{
    participants.insert(new_participant);

    response::Response resp;
    response::Response::JoinState state;
    std::cout << "Join State created" << std::endl;
    *resp.mutable_join_state() = state;
    std::cout << "delivered" << std::endl;
    host->deliver(resp);
}

void SessionRoom::leave(const ConnectionShPtr participant_to_leave)
{
    participants.erase(participant_to_leave);
}

void SessionRoom::deliver(const response::Response& resp)
{
    for (auto participant : participants)
        participant->deliver(resp);
}