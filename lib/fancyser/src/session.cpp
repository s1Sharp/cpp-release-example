#include <sstream>
#include "spdlog/spdlog.h"
#include "server.hpp"
#include "session.hpp"
#include "message.hpp"
#include "utils.hpp"

session_t::session_t(shared_ptr<io_service> ios, server_t* server_ptr) noexcept
	: m_io_service{ios},
	  m_socket{*m_io_service},
	  m_timer(*m_io_service),
	  m_server_ptr{server_ptr}
{
}

session_t::~session_t() noexcept
{
	stop();
}

bool session_t::start() noexcept
{
	spdlog::info("session started");
	if (!process())
	{
		return false;
	}
	spdlog::info("session completed");
	return true;
} 

void session_t::stop() noexcept
{
	if (m_socket.is_open())
	{
		m_socket.shutdown(boost::asio::socket_base::shutdown_both);
		m_socket.close();
	}
}

void session_t::setIdentity(const client_identity_t& identity)
{
	m_ci.client_ip = identity.client_ip;
	m_ci.client_port = identity.client_port;
}

session_t::client_identity_t session_t::getIdentity() const
{
	return m_ci;
}

bool session_t::process() noexcept
{
	if (!welcome_client())
	{
		return false;
	}

	m_last_ping = boost::posix_time::microsec_clock::local_time();
	on_check_ping();
	post_check_ping();

	do
	{
		spdlog::info("receiving command message from client");
		error_code e;
		const auto msg = recv(m_socket, e);
		if (e) {
			handle_error(e);
			return false;
		}
		if ((msg.get_header().get_type() == message_t::type_t::exit) &&
			(msg.get_payload() == "EXIT"))
		{
			spdlog::info("exit message received");
			break;
		}
		else if (msg.get_header().get_type() == message_t::type_t::command_request)
		{
			if (!process_command(msg.get_payload()))
			{
				spdlog::info("error while process command request message");
				return false;
			}
			spdlog::info("command processing completed");
		}
		else if (msg.get_header().get_type() == message_t::type_t::ping)
		{
			if (msg.get_header().is_valid()) {
				post_check_ping();
				spdlog::info("command processing completed");
			} else {
				spdlog::info("command processing failed, incorrect message header");
			}
		}
		else
		{
			spdlog::error("invalid message type received");
			return false;
		}
	} while (m_socket.is_open());
	return true;
}


bool session_t::do_ping() noexcept
{
	message_t cmd_ping_msg;
	cmd_ping_msg.set(message_t::type_t::ping, "ping");
	if (!send(m_socket, cmd_ping_msg))
	{
		spdlog::error("error while sending ping message");
		return false;
	}
	m_timer.expires_from_now(boost::posix_time::millisec(5000)); // Устанавливаем таймер на 5 секунд
	m_timer.async_wait(boost::bind(&session_t::on_check_ping, shared_from_this()));
	spdlog::info("do_ping");
	return true;
}

void session_t::on_check_ping() noexcept
{
	const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
	if ((now - m_last_ping).total_milliseconds() > 5000)
	{
		spdlog::info("stopping {} - no ping in time", std::string(m_ci));
		m_server_ptr->obs().removeSession(getIdentity(), shared_from_this());
		return;
	}

	m_last_ping = boost::posix_time::microsec_clock::local_time();
	spdlog::info("on_check_ping");
}

void session_t::post_check_ping() noexcept
{
	// Перезапускаем таймер
	m_timer.expires_from_now(boost::posix_time::millisec(5000));
	m_timer.async_wait(boost::bind(&session_t::on_check_ping, shared_from_this()));
	spdlog::info("post_check_ping");
}

void session_t::handle_error(const boost::system::error_code& error)
{
	if (error == boost::asio::error::eof)
	{
		// End of file reached
		spdlog::info("End of file reached, client disconnected");
		m_server_ptr->obs().removeSession(getIdentity(), shared_from_this());
	}
	else if (error)
	{
		// Some other error occurred
		spdlog::info("Unknown error: {}", error.what());
	}
}

bool session_t::welcome_client() noexcept
{
	std::ostringstream oss;
	oss << "Welcome! [" << get_peer_ip(m_socket) << ":" << get_peer_port(m_socket) << "]";

	message_t welcome_msg;
	welcome_msg.set(message_t::type_t::welcome, oss.str());
	spdlog::info("sending welcome message to client");
	if (!send(m_socket, welcome_msg))
	{
		spdlog::error("error while sending welcome message");
		return false;
	}
	spdlog::info("welcome message sent");
	return true;
}

bool session_t::process_command(const std::string &cmd) noexcept
{
	spdlog::info("processing command [{}]", cmd);

	const auto exit_status = std::system(cmd.data());
	const auto cmd_response_payload = (exit_status == 0 ? "SUCCESS" : "FAILURE");

	spdlog::info("sending command response message");
	message_t cmd_response_msg;
	cmd_response_msg.set(message_t::type_t::command_response, cmd_response_payload);
	if (!send(m_socket, cmd_response_msg))
	{
		spdlog::error("error while sending command response message");
		return false;
	}
	spdlog::info("command response message sent");
	return true;
}
