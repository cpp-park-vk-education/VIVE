#include "Connection.hpp"

Connection::Connection(tcp::socket socket, SignalManagerShPtr room_manager) : socket_(std::move(socket)),
                                                                              room_manager_(room_manager),
                                                                              room_actions(std::make_unique<RoomSignals>())
{
    //room_actions = std::make_unique<RoomSignals>();
    std::cout << "ROOM MANAGER IS " << (room_manager_ == nullptr) << std::endl;
    room_actions->connect(*room_manager_);
    std::cout << "ROOM ACTIONS IS " << (room_actions == nullptr) << std::endl;
    std::cout << "Building connection" << std::endl;
    buf_to_read_in_.prepare(1024);
}

Connection::~Connection()
{
    socket_.close();
}

void Connection::start()
{
    std::cout << "Connection started" << std::endl;
    room_manager_->setConnection(shared_from_this());
    doReadHeader();
}

int Connection::decodeHeader(std::string& header)
{
    header.erase(header.begin(), 
                std::find_if(header.begin(), header.end(), [](unsigned char ch) {return !std::isspace(ch);}));

    return std::stoi(header);
}

void prepareMessageToWrite(std::string& message)
{
    std::string header(4, ' ');
    std::string size_string = std::to_string(static_cast<int>(message.size()));

    for (int i = size_string.size() - 1; i >= 0; --i)
        header[i + (header.size() - size_string.size())] = size_string[i];

    std::cout << message.size() << " " << header << std::endl;

    message.insert(0, header);

}

void Connection::doReadHeader()
{
    std::cout << "Waiting for reading header..." << std::endl;
    boost::asio::async_read
    (
        socket_,
        buf_to_read_in_,
        boost::asio::transfer_exactly(header_length),
        [this](boost::system::error_code error, size_t /*length*/)
        {
            try
            {
                if (!error)
                {
                    std::string string_buf{buffers_begin(buf_to_read_in_.data()), 
                                           buffers_end(buf_to_read_in_.data())};

                    buf_to_read_in_.consume(string_buf.size());
                    buf_to_read_in_.prepare(1024);
                
                    std::cout << "Header read " << string_buf << std::endl;
                    int body_size = decodeHeader(string_buf);
                    doReadBody(body_size);
                }
                else
                    std::cerr << "Header reading failed: " << error.message() << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception during reading header: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "Some exception during reading header" << std::endl;
            }
        }
    );
}

void Connection::doReadBody(int body_size)
{
    std::cout << "Waiting for reading body" << std::endl;
    boost::asio::async_read
    (
        socket_,
        buf_to_read_in_,
        boost::asio::transfer_exactly(body_size),
        [this](boost::system::error_code error, size_t /*length*/)
        {
            try
            {
                if (!error)
                {
                    std::string string_buf{buffers_begin(buf_to_read_in_.data()), 
                                           buffers_end(buf_to_read_in_.data())};

                    buf_to_read_in_.consume(string_buf.size());
                    buf_to_read_in_.prepare(1024);

                    request_.ParseFromString(string_buf);

                    std::cout << "Body read " << string_buf.size() << std::endl;
                    sendSignal();
                    doReadHeader();
                }
                else
                    std::cerr << "Body reading failed: " << error.message() << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception during reading body: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "Some exception during reading body" << std::endl;
            }
        }
    );
}

void Connection::sendSignal()
{
    bool result = false;
    if (request_.has_join_state())
    {
        std::cout << "Object has join state " << std::stoi(request_.join_state().code()) << std::endl;
        bool result = *room_actions->joinRoom(std::stoi(request_.join_state().code()));
        
        std::cout << "Room created" << std::endl;
        response::Response::JoinState state;
        state.set_ok(result);

        std::cout << "State set" << std::endl;
        response::Response response_;
        *response_.mutable_join_state() = state;
        std::cout << "Ready to send response" << std::endl;
        sendResponse(response_);

        return;
    }

    if (request_.has_init_multiplayer_state())
    {
        std::cout << "Initiating multiplayer" << std::endl;
        int result = *(room_actions->requestNewRoom());

        auto state = new response::Response::InitMultiplayerState;
        state->set_code(std::to_string(result));
        
        std::cout << "Room ID " << result << std::endl;

        response::Response response_;
        response_.set_allocated_init_multiplayer_state(state);
        sendResponse(response_);

        return;
    }

    /*switch (request_.join_state().code())
    {
    case 1:
        room_actions->requestNewRoom();
        break;
    case 2:
        room_actions->joinRoom(std::atoi(read_mess.getBody()));
    case 3:
        room_actions->leaveRoom();
    default:
        std::cerr << "Invalid request!" << std::endl;
        break;
    }*/
}

void Connection::sendResponse(const response::Response& resp)
{
    bool write_in_progress = !resp_queue_to_write.empty();
    resp_queue_to_write.push(resp);

    if (!write_in_progress)
        doWrite();
}

void Connection::doWrite()
{
    auto self(shared_from_this());
    auto resp_to_write = resp_queue_to_write.front();

    std::string string_buf;
    resp_to_write.SerializeToString(&string_buf);
    prepareMessageToWrite(string_buf);

    boost::asio::async_write
    (
        socket_,
        boost::asio::buffer(string_buf.data(), string_buf.size()),
        [this, self](boost::system::error_code error, std::size_t /*length*/)
        {
            try
            {
                if (!error)
                {
                    std::cout << "Write done" << std::endl;
                    resp_queue_to_write.pop();
    
                    if (!resp_queue_to_write.empty())
                    {
                        doWrite();
                    }
                }
                else
                    std::cout << "Writing failed: " << error.message() << std::endl;
            }
            catch (std::exception& e)
            {
                std::cerr << "Exception during writing: " << e.what() << std::endl;
            }
            catch (...)
            {
                std::cerr << "Some exception during writing" << std::endl;
            }
        }
    );
}

void Connection::deliver(const response::Response& resp)
{
    bool write_in_progress = !resp_queue_to_write.empty();
    resp_queue_to_write.push(resp);

    if (!write_in_progress)
        doWrite();
}

void Connection::close()
{
    socket_.close();
}