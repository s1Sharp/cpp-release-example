#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <thread>

#include <boost/asio.hpp>

using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::system::error_code;

class client_t final
{
public:
	client_t(const std::string ip, const unsigned short port) noexcept;
	~client_t() noexcept;

	bool start() noexcept;
	bool process_cmd_input(const std::string& cmd, std::string& response);

private:
	bool connect() noexcept;
	bool process() noexcept;
	bool exit() noexcept;
	void disconnect() noexcept;
	void run_thread_ping() noexcept;

	void handle_error(const boost::system::error_code& error);

	io_service m_io_service;
	tcp::endpoint m_endpoint;
	tcp::socket m_socket;

	bool m_running_thread;
	std::thread m_ping_thread;

    std::ostream* m_out_stream;
};

class client_console {
	
};

#endif
