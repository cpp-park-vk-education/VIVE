#include <gtest/gtest.h>
#include "Server.hpp"

TEST(ServerInitTest, BasicCase)
{
    boost::asio::io_context io_context;
    tcp::endpoint endpoint(tcp::v4(), 12345);
    Server s(io_context, endpoint);
    //Message mess("qwerty");
    EXPECT_EQ(5 * 5, 25);
}