//  Copyright © 2016 George Georgiev. All rights reserved.
//

#include <boost/asio.hpp> // IWYU pragma: keep
#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <type_traits>

using boost::asio::ip::tcp;

namespace rpc
{
//----------------------------------------------------------------------

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start()
    {
        do_read_header();
    }

    void deliver()
    {
        // bool write_in_progress = !write_msgs_.empty();
        // write_msgs_.push_back(msg);
        // if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
#if 0    
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(),
                                                    chat_message::header_length),
                                [this, self](boost::system::error_code ec,
                                             std::size_t /*length*/) {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
#endif
    }

    void do_read_body()
    {
#if 0    
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(),
                                                    read_msg_.body_length()),
                                [this, self](boost::system::error_code ec,
                                             std::size_t /*length*/) {
                                    if (!ec)
                                    {
                                        room_.deliver(read_msg_);
                                        do_read_header();
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
#endif
    }

    void do_write()
    {
#if 0     
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this, self](boost::system::error_code ec,
                                              std::size_t /*length*/) {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         room_.leave(shared_from_this());
                                     }
                                 });
#endif
    }

    tcp::socket socket_;
};

//----------------------------------------------------------------------

class chat_server
{
public:
    chat_server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint)
        , socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!ec)
            {
                // std::make_shared<chat_session>(std::move(socket_), room_)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

//----------------------------------------------------------------------

int mainServer(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_service, endpoint);
        }

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
}
