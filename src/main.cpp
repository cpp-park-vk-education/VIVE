#include "Server.hpp"

#include <iostream>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Use parameters: <port> to run the Server" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
        auto server = std::make_shared<Server>(io_context, endpoint);

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception " << e.what() << std::endl;
    }
}