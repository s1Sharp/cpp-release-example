#ifndef UTILS_HPP
#define UTILS_HPP

#include <stdexcept>
#include <string>

#include <boost/asio.hpp>

#include "message.hpp"

using boost::asio::ip::tcp;
using boost::system::error_code;

namespace utils {

class BrokenPipeSocketException : public std::runtime_error {
public:
    BrokenPipeSocketException(const std::string& message)
        : std::runtime_error(message) {}
};


std::string get_peer_ip(const tcp::socket &socket) noexcept;
unsigned short get_peer_port(const tcp::socket &socket) noexcept;

message_t recv(tcp::socket &socket, error_code& e);
bool send(tcp::socket &socket, const message_t &message);

}

#endif
