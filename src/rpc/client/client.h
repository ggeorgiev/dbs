//  Copyright © 2016 George Georgiev. All rights reserved.
//

#pragma once

#include <boost/asio/io_service.hpp>

namespace rpc
{
class Client
{
public:
    Client();

    bool connect(int port);

private:
    boost::asio::io_service mIoService;
};
}
