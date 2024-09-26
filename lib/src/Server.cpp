#include "Server.hpp"

Server::Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint) : acceptor_(io_context, endpoint), highest_room_id(0)
{
    doAccept();
}

void Server::doAccept()
{
    std::cout << "Waiting for connection..." << std::endl;
    acceptor_.async_accept
    (
        [this](boost::system::error_code error, tcp::socket socket)
        {
            if (!error)
            {
                std::cout << "Connected" << std::endl;
                auto new_connection = std::make_shared<Connection>(std::move(socket), std::make_shared<SignalManager>(shared_from_this()));
                free_connections.insert(new_connection);
                new_connection->start();
            }
            else
                std::cerr << "Connection failed: " << error.message() << std::endl;

            doAccept();
        }
    );
}

std::pair<RoomShPtr, int> Server::createNewRoom(ConnectionShPtr connection)
{
    std::cout << "Connection is Invalid: " << (connection == nullptr) << std::endl;
    RoomShPtr new_room = std::make_shared<SessionRoom>(connection);
    bool success = id2room.insert(std::make_pair(++highest_room_id, new_room)).second;

    if (!success)
        return std::make_pair(nullptr, 0);

    //new_room->join(connection);
    free_connections.erase(connection);
    std::cout << "Free connections size: " << free_connections.size() << std::endl;

    return std::make_pair(new_room, highest_room_id);
}

RoomShPtr Server::getRoom(int room_id, ConnectionShPtr connection)
{
    std::cout << "Server get room " << room_id << std::endl;
    auto result = id2room.find(room_id);
    
    if (result == id2room.end())
    {
        std::cout << "Not found" << std::endl;
        return nullptr;
    }

    std::cout << "Found" << std::endl;
    result->second->join(connection);
    free_connections.erase(connection);

    return result->second;
}