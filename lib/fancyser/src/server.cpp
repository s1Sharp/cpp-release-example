#include <exception>
#include <cstdlib>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/system/error_code.hpp>
#include <boost/lexical_cast.hpp>

#include <spdlog/spdlog.h>

#include "db_service.hpp"
#include "server.hpp"
#include "utils.hpp"

using boost::shared_ptr;
using boost::asio::ip::address;
using boost::asio::placeholders::error;
using boost::system::error_code;


void session_observer::addSession(session_t::client_identity_t ci, boost::shared_ptr<session_t> session)
{
	m_client_identities.emplace(std::make_pair(ci, session));
}

void session_observer::removeSession(session_t::client_identity_t ci, boost::shared_ptr<session_t> session)
{
	spdlog::info("removeSession [{}:{}]", ci.client_ip, ci.client_port);
	m_client_identities.erase(ci);
	session->stop();
	session.reset();
}

void session_observer::stopObserve()
{
	for (auto [ci, session] : m_client_identities) {
		session->stop();
		session.reset();
		spdlog::info("stop observe session [{}:{}]", ci.client_ip, ci.client_port);
	}
	m_client_identities.clear();
}

server_t::server_t(
	const std::string ip, const unsigned short port,
	const unsigned short num_threads) noexcept
	: m_ios_acceptors{boost::make_shared<io_service>()},
	  m_ios_work_acceptors{boost::make_shared<io_service::work>(*m_ios_acceptors)},
	  m_ios_executors{boost::make_shared<io_service>()},
	  m_ios_work_executors{boost::make_shared<io_service::work>(*m_ios_executors)},
	  m_endpoint{address::from_string(ip), port},
	  m_acceptor{*m_ios_acceptors, m_endpoint},
	  m_session{boost::make_shared<session_t>(m_ios_executors, this)},
	  m_signals{*m_ios_acceptors, SIGINT, SIGTERM}
{
	// Add signal handling for graceful termination (CTRL + C)
	m_signals.async_wait(boost::bind(&server_t::stop, this));

	spdlog::info("starting [{}:{}]", ip, port);

	for (unsigned int i = 0; i < num_threads; ++i)
	{
		m_executors_thread_group.create_thread(boost::bind(&server_t::worker_thread_callback,
														   this,
														   m_ios_executors));
	}

	m_acceptor.async_accept(m_session->get_socket(),
							boost::bind(&server_t::accept_handler, this, m_session, error));


	std::string fancy_redis_url = "tcp://default:redispw@localhost:6379";
	if (const char* env_p = std::getenv("FANCY_REDIS_URL")) {
		fancy_redis_url = env_p;
	}
	auto redisDb = boost::make_shared<RedisDbService>(fancy_redis_url, "");
	m_db_service = boost::make_shared<DbServiceWrapper>(redisDb);

	spdlog::info("started, press CTRL+C to quit");
}

server_t::~server_t() noexcept
{
	stop();
	spdlog::info("server stopped successfully");
}

bool server_t::start() noexcept
{
	error_code ec;
	m_ios_acceptors->run(ec);
	if (ec)
	{
		spdlog::error("error: {}", ec.message());
		return false;
	}
	return true;
}

void server_t::stop() noexcept
{
	if (!m_ios_acceptors->stopped())
	{
		spdlog::error("resived signal to stop acceptors");
		m_ios_acceptors->stop();
	}

	if (!m_ios_executors->stopped())
	{
		spdlog::error("resived signal to stop executors");
		m_ios_executors->stop();
		m_session_observer.stopObserve();
		m_executors_thread_group.interrupt_all();
		m_executors_thread_group.join_all();
	}
}

session_observer& server_t::obs()
{
	return m_session_observer;
}

shared_ptr<DbServiceWrapper> server_t::GetDbService()
{
	return m_db_service;
}

// Utility methods

void server_t::worker_thread_callback(boost::shared_ptr<io_service> ios) noexcept
{
	error_code ec;
	ios->run(ec);
	spdlog::info("worker thread ended: thread id {}", boost::lexical_cast<std::string>(boost::this_thread::get_id()));
	if (ec)
	{
		spdlog::error("callback error: {}", ec.message());
	}
}

void server_t::accept_handler(boost::shared_ptr<session_t> this_session, const error_code &ec) noexcept
{
	if (!ec)
	{
		const auto client_ip = utils::get_peer_ip(this_session->get_socket());
		const auto client_port = utils::get_peer_port(this_session->get_socket());
		spdlog::info("new client connected [{}:{}]", client_ip, client_port);

		m_ios_executors->post(boost::bind(&session_t::start, this_session));

		const session_t::client_identity_t client_identity{client_ip, client_port};
		this_session->setIdentity(client_identity);
		m_session_observer.addSession(client_identity, this_session);
		spdlog::info("new client connected [{}:{}]", client_ip, client_port);
		m_session = boost::make_shared<session_t>(m_ios_executors, this);

		m_acceptor.async_accept(m_session->get_socket(),
								boost::bind(&server_t::accept_handler, this, m_session, error));
	}
}
