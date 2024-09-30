# WorLibrary::Network

Library contains network (tcp/udp types) client/server implementation. It's constructed via `boost::async` methods.

In progress:
- Serial port
- ArtNet protocol (it will be placed in `Wor::Protocols` module with `FreeD` maybe)

All classes have common method like `start`, `bound` and etc, but It seems unreasonable, 
despite the fact that I really want to. Perhaps some abstract parent will be implemented for all socket operations,
but this is not relevant at the moment.

---

## Dependencies

- WorLibrary
    - Log [v0.1]()
- boost [v1.86.0](https://github.com/boostorg/boost/tree/boost-1.86.0)

---

## Starting tcp client

```c++
#include "Wor/Network/TcpClient.hpp"
#include "Wor/Network/Utils/IoService.hpp"
#include "Wor/Wrappers/Singleton.hpp"

int main() {
    boost::asio::io_context &io = Utils::IoService::get();

    TcpClient client(io);
    boost::asio::ip::tcp::resolver end_point(io);
    const boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> end_point_it 
        = end_point.resolve(boost::asio::ip::tcp::resolver::query("169.254.8.253", "5250"));
   if (client.start(end_point_it); !client.bound()) {
       ...
       return ...;
   }

    client.receiveCallback([](std::string message) {
        ...
    });
    Network::Utils::IoService::run();
    ...
    return 0;
}
```

--- 

## Starting tcp server

```c++
#include "Wor/Network/TcpServer.hpp"
#include "Wor/Network/Utils/IoService.hpp"
#include "Wor/Wrappers/Singleton.hpp"

int main() {	
    Wor::Network::TcpServer::Endpoint local_end_point;
    local_end_point.port(33000);
    boost::asio::ip::address address{boost::asio::ip::make_address_v4("127.0.0.1")};
    local_end_point.address(address);

    auto &server = Wor::Wrappers::Singleton<Wor::Network::TcpServer>::get();

    if (!server.bindTo(local_end_point)) {
        server.stop();
        ...
        return ...;
    }
    if (server.start(); !server.bound()) {
        ...
        return ...;
    }
    
    Network::Utils::IoService::run();
    ...
    return 0;
}
```

---

## Starting udp server

```c++
#include "Wor/Network/UdpServer.hpp"
#include "Wor/Network/Utils/IoService.hpp"
#include "Wor/Wrappers/Singleton.hpp"

int main() {
    Wor::Network::UdpServer::Endpoint endpoint;
    endpoint.port(6001);
    boost::asio::ip::address end_point_address{boost::asio::ip::make_address_v4(address)};
    endpoint.address(end_point_address);
    Wor::Network::UdpServer server;
    server.start(endpoint);
    if (!server.bound()) {
        ...
        return ...;
    }
	
    Wor::Network::Utils::IoService::run();
    ...
    return 0;
}
```

---

### TODO: fill doc about TcpSession, sending packets and etc.

---

### by Worshiper Hyakki Yakō(worHyako)
