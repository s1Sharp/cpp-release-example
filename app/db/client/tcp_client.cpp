#include "tcp_client.hpp"
#include <future>
#include <boost/asio/use_future.hpp>
#include <array>
#include <iostream>

namespace network
{
constexpr uint8_t BUFFER_SIZE = 128;

tcp_client::tcp_client(const std::string& ip_addr, uint16_t port) :
    ip_addr_(ip_addr), port_num_(port), work_(io_service_), tcp_socket_(io_service_)
{
    // Create a thread that operates completely asynchronously with respect to the rest of program.
    thread_ = std::make_unique<std::thread>([this](){ io_service_.run(); });
}

tcp_client::~tcp_client()
{
    io_service_.stop();
    thread_->join();
}

void tcp_client::send_request(const std::string& request)
{
    std::future<void> connect_future;
    try
    {
        basio::ip::tcp::resolver resolver(io_service_);

        // The async_resolve operation returns the endpoint iterator as a future value.
        std::future<boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp>> iter =
            resolver.async_resolve(
                {basio::ip::tcp::v4(), ip_addr_, std::to_string(port_num_)},
                boost::asio::use_future);

        connect_future = tcp_socket_.async_connect(*iter.get(),
                                                               boost::asio::use_future);
        connect_future.get();

        std::future<std::size_t> send_length =
            tcp_socket_.async_send(boost::asio::buffer(request),
                             boost::asio::use_future);
        // Other things can be done here while the send completes.
        send_length.get(); // blocks until the send is complete.

        std::array<char, BUFFER_SIZE> recv_buf;
        std::future<std::size_t> recv_length =
            tcp_socket_.async_receive(
                boost::asio::buffer(recv_buf),
                boost::asio::use_future);

        // Other things can be done here while the receive completes.
        std::cout.write(
            recv_buf.data(),
            recv_length.get()); // blocks until receive is complete.
    }
    catch (const std::exception& e)
    {
        std::cerr << std::boolalpha;
        std::cerr << " caught exception " << e.what() << ", connectoin is valid = " << connect_future.valid() << std::endl;
    }
}

}

