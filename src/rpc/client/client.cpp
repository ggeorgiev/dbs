//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include "rpc/client/client.h"
#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>

namespace rpc
{
using boost::asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock)
{
    try
    {
        for (;;)
        {
            char data[max_length];

            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            boost::asio::write(sock, boost::asio::buffer(data, length));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

bool Client::connect(int port)
{
    tcp::acceptor acceptor(mIoService, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        tcp::socket sock(mIoService);

        acceptor.accept(sock);
        std::thread(session, std::move(sock)).detach();
    }
}
}
