#pragma once

#include <string>
#include <boost/asio.hpp>
#include "message.hpp"

using boost::asio::ip::tcp;
using boost::system::error_code;

std::string get_peer_ip(const tcp::socket &socket) noexcept;
unsigned short get_peer_port(const tcp::socket &socket) noexcept;

message_t recv(tcp::socket &socket, error_code& e) noexcept;
bool send(tcp::socket &socket, const message_t &message) noexcept;
