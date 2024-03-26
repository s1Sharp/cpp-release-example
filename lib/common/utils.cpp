#include <boost/system/error_code.hpp>

#include <spdlog/spdlog.h>
#include "utils.hpp"

namespace utils {




std::string get_peer_ip(const tcp::socket &socket) noexcept
{
	return socket.remote_endpoint().address().to_string();
}

unsigned short get_peer_port(const tcp::socket &socket) noexcept
{
	return socket.remote_endpoint().port();
}

message_t recv(tcp::socket &socket, boost::system::error_code& e)
{
	spdlog::debug("receiving message");

	message_t::header_t header;

	boost::system::error_code& ec = e;
	boost::asio::read(socket, boost::asio::buffer(&header, sizeof(header)), boost::asio::transfer_exactly(sizeof(header)), ec);
	if (ec)
	{
		spdlog::error("header read error, {}", ec.message());
		if (ec == boost::asio::error::eof)
		{
	
			spdlog::error("End of file reached");
			return {};
		}
		return {};
	}

	spdlog::debug("header: {}", header.dump());

	if (!header.is_valid())
	{
		spdlog::error("invalid header");
		return {};
	}

	if (header.get_payload_size() == 0)
	{
		return {header, ""};
	}

	boost::asio::streambuf payload_buffer;
	boost::asio::read(socket, payload_buffer, boost::asio::transfer_exactly(header.get_payload_size()), ec);
	if (ec)
	{
		spdlog::error("payload read error, {}", ec.message());
		// Проверка, является ли ошибка разрывом соединения
		if (ec == boost::asio::error::connection_reset || ec == boost::asio::error::broken_pipe) {
			// Закрытие сокета и завершение работы
			socket.close();
			throw BrokenPipeSocketException(ec.message());
		}
		return {};
	}

	const std::string payload((std::istreambuf_iterator<char>(&payload_buffer)),
							  std::istreambuf_iterator<char>());

	const auto message = message_t{header, payload};
	spdlog::debug("message received: {}", message.dump());
	return message;
}

bool send(tcp::socket &socket, const message_t &message)
{
	spdlog::debug("sending message: {}", message.dump());

	const auto header = message.get_header();
	boost::system::error_code ec;
	boost::asio::write(socket, boost::asio::buffer(&header, sizeof(header)), boost::asio::transfer_exactly(sizeof(header)), ec);
	if (ec)
	{
		spdlog::error("header send error, {}", ec.message());

		// Проверка, является ли ошибка разрывом соединения
		if (ec == boost::asio::error::connection_reset || ec == boost::asio::error::broken_pipe) {
			// Закрытие сокета и завершение работы
			socket.close();
			throw BrokenPipeSocketException(ec.message());
		}

		return false;
	}

	if (header.get_payload_size() > 0)
	{
		const auto payload = message.get_payload();
		boost::asio::write(socket, boost::asio::buffer(payload, payload.size()), boost::asio::transfer_exactly(payload.size()), ec);
		if (ec)
		{
			spdlog::error("payload send error, {}", ec.message());
			return false;
		}
	}

	spdlog::debug("message sent [{}:{}]", utils::get_peer_ip(socket), utils::get_peer_port(socket));
	return true;
}

} // namespace utils