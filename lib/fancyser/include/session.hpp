#ifndef SESSION_HPP
#define SESSION_HPP

#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::shared_ptr;
using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::enable_shared_from_this;

class session_observer;
class server_t;

class session_t final : public enable_shared_from_this<session_t>
{
public:
	struct client_identity_t
	{
		client_identity_t() {}
		client_identity_t(const std::string ip, const uint16_t port) : client_ip(ip), client_port(port) {}

		bool operator<(const client_identity_t& rhs) const
		{
			return client_ip < rhs.client_ip && client_port < rhs.client_port;
		}
		explicit operator std::string() const { return client_ip + ":" + std::to_string(client_port); }

		std::string client_ip;
		uint16_t client_port;
	};
public:
	session_t(shared_ptr<io_service> ios, server_t* server_ptr) noexcept;
	~session_t() noexcept;

	bool start() noexcept;
	void stop() noexcept;

	void setIdentity(const client_identity_t& identity);
	client_identity_t getIdentity() const;

	tcp::socket &get_socket() noexcept { return m_socket; }

private:
	bool process() noexcept;
	bool welcome_client() noexcept;
	bool process_command(const std::string &cmd) noexcept;

	bool do_ping() noexcept;
	void on_check_ping()  noexcept;
	void post_check_ping() noexcept;

	void handle_error(const boost::system::error_code& error);

	shared_ptr<io_service> m_io_service;
	tcp::socket m_socket;
	server_t* m_server_ptr;

	client_identity_t m_ci;
	boost::asio::deadline_timer m_timer;
	boost::posix_time::ptime m_last_ping;
};

#endif
