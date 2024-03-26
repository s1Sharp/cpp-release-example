#ifndef SERVER_HPP
#define SERVER_HPP

#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "db_service.hpp"
#include "session.hpp"

using boost::enable_shared_from_this;
using boost::shared_ptr;
using boost::asio::io_service;
using boost::asio::ip::tcp;
using boost::system::error_code;

struct session_observer {
	void addSession(session_t::client_identity_t ci, shared_ptr<session_t> session);
	void removeSession(session_t::client_identity_t ci, shared_ptr<session_t> session);
	void stopObserve();
private:
	std::map< session_t::client_identity_t, shared_ptr<session_t> > m_client_identities;
};

class server_t final
{
public:
	server_t(
		const std::string ip, const unsigned short port,
		const unsigned short num_threads) noexcept;
	~server_t() noexcept;

	bool start() noexcept;
	void stop() noexcept;

	session_observer& obs();

	shared_ptr<DbServiceWrapper> GetDbService();

private:
	void worker_thread_callback(shared_ptr<io_service> ios) noexcept;
	void accept_handler(shared_ptr<session_t> this_session, const error_code &ec) noexcept;
	void accept_new_connection() noexcept;

	shared_ptr<io_service> m_ios_acceptors;
	shared_ptr<io_service::work> m_ios_work_acceptors;
	shared_ptr<io_service> m_ios_executors;
	shared_ptr<io_service::work> m_ios_work_executors;
	boost::thread_group m_executors_thread_group;

	tcp::endpoint m_endpoint;
	tcp::acceptor m_acceptor;

	session_observer m_session_observer;
	shared_ptr<session_t> m_session;

	boost::asio::signal_set m_signals;

	shared_ptr<DbServiceWrapper> m_db_service;
};

#endif
