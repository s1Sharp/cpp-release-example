#include <memory>
#include <algorithm>

#include <spdlog/spdlog.h>

#include <cli/cli.h>
#include <cli/loopscheduler.h>
#include <cli/boostasioscheduler.h>
#include <cli/filehistorystorage.h>
#include <cli/clilocalsession.h>
#include <numeric>

#include "client.hpp"
#include "message.hpp"
#include "utils.hpp"
#include "defaults.hpp"


using boost::asio::ip::address;
using boost::asio::placeholders::error;
using boost::system::error_code;

client_t::client_t(const std::string ip, const unsigned short port) noexcept
	: m_endpoint{address::from_string(ip), port},
	  m_socket{m_io_service},
	  m_running_thread(false)
{
}

client_t::~client_t() noexcept
{
	m_running_thread = false;
	disconnect();
	if (m_ping_thread.joinable()) {
		m_ping_thread.join();
	}
}

bool client_t::start() noexcept
{
	if (!connect())
	{
		return false;
	}
    run_thread_ping();
	if (!process())
	{
		return false;
	}
	return true;
}

bool client_t::connect() noexcept
{
	spdlog::info("connecting [{}:{}]", m_endpoint.address().to_string(), m_endpoint.port());

	error_code ec;
	m_socket.connect(m_endpoint, ec);
	if (ec)
	{
		spdlog::error("failed to connect, error: {}", ec.message());
		return false;
	}

	// Waiting welcome message
	error_code e;
	auto welcome_msg = utils::recv(m_socket, e);
	if (e) {
		handle_error(e);
		return false;
	}
	while(welcome_msg.get_header().get_type() != message_t::type_t::welcome) {
		spdlog::warn("expect recieving welcome message from server, but got another message type: {}", welcome_msg.get_header().get_type_as_string());
		welcome_msg = utils::recv(m_socket, e);
		if (e) {
			handle_error(e);
			return false;
		}
	}

	spdlog::info("welcome message received, {}", welcome_msg.dump());
	spdlog::info("connected");
	return true;
}

void client_t::run_thread_ping() noexcept
{
	m_running_thread = true;
	m_ping_thread = std::thread([this]() {
		while(m_running_thread) {
			std::this_thread::sleep_for(std::chrono::seconds(defaults::server::BASE_PING_INTEVAL_SEC));
			if (!m_running_thread) {
				return;
			}
			message_t cmd_request_msg;
			cmd_request_msg.set(message_t::type_t::ping, "ping");
			spdlog::debug("sending command request [{}]", cmd_request_msg.dump());
            try {
                if (!utils::send(m_socket, cmd_request_msg))
                {
                    spdlog::error("error while sending ping");
                }
            } catch (const utils::BrokenPipeSocketException& bpe) {
                std::cerr << "Error: " << bpe.what() << std::endl;
                std::cerr << "Shutdown client" << std::endl;
                ::exit(1);
            } catch (const std::exception& e) {
                std::cerr << "Fatal error: " << e.what() << std::endl;
                ::exit(2);
            }

		}
	});
}

bool client_t::process_cmd_input(const std::string& cmd, std::string& response)
{
 	// Send command request message
    message_t cmd_request_msg;
    cmd_request_msg.set(message_t::type_t::command_request, cmd);
    spdlog::debug("sending command request [{}]", cmd_request_msg.dump());
    if (!utils::send(m_socket, cmd_request_msg))
    {
        spdlog::error("error while sending command request");
        return false;
    }
    spdlog::debug("command request sent");

    // Receive command response message
    boost::system::error_code e;
    spdlog::debug("receiving command response from server");
    auto cmd_response_msg = utils::recv(m_socket, e);
    if (e) {
        handle_error(e);
        return false;
    }
	while (cmd_response_msg.get_header().get_type() == message_t::type_t::ping) {
		auto cmd_response_msg = utils::recv(m_socket, e);
		if (e) {
			handle_error(e);
			return false;
		}
	}
    if (cmd_response_msg.get_header().get_type() != message_t::type_t::command_response)
    {
        spdlog::info("invalid command response type, {}", cmd_response_msg.dump());
        return false;
    }
    spdlog::debug("command response received: {}", cmd_response_msg.dump());

	response = cmd_response_msg.get_payload();
	return true;
}

class IFancyCommand {

};
// a custom struct to be used as a user-defined parameter type
struct SetFancyCommand : public IFancyCommand
{
    // [[nodiscard]] std::string to_string() const {
	// 	return m_command + std::accumulate(m_command_args.begin(), m_command_args.end(), std::string(),
	// 										[](const std::string& a, const std::string& b) {
	// 											return a + (a.empty() ? "" : " ") + b;
	// 										});
	// }
	SetFancyCommand(std::string key, std::string value) : m_command("set"), m_key(key), m_value(value) {}

    [[nodiscard]] std::string to_string() const {
		return m_command + " " + m_key + " " + m_value;
	}
    friend std::istream & operator >> (std::istream &in, SetFancyCommand& p);
private:
    std::string m_command;
    std::string m_key;
    std::string m_value;
};

struct GetFancyCommand : public IFancyCommand
{
	GetFancyCommand(std::string key) : m_command("get"), m_key(key) {}

    [[nodiscard]] std::string to_string() const {
		return m_command + " " + m_key;
	}
    friend std::istream & operator >> (std::istream &in, GetFancyCommand& p);
private:
    std::string m_command;
    std::string m_key;
};

std::istream &operator>>(std::istream &in, SetFancyCommand &p) {
    in >> p.m_command;
	in >> p.m_key;
	in >> p.m_value;
    // while(in) {
    //     std::string sw;
    //     in >> sw;
    //     p.m_command_args.emplace_back(std::move(sw));
    // }
    return in;
}

std::istream &operator>>(std::istream &in, GetFancyCommand &p) {
    in >> p.m_command;
	in >> p.m_key;
    // while(in) {
    //     std::string sw;
    //     in >> sw;
    //     p.m_command_args.emplace_back(std::move(sw));
    // }
    return in;
}


bool client_t::process() noexcept
{
    using namespace cli;
    // create a cli with the given root menu and a persistent storage
    // you must pass to FileHistoryStorage the path of the history file
    // if you don't pass the second argument, the cli will use a VolatileHistoryStorage object that keeps in memory
    // the history of all the sessions, until the cli is shut down.
    auto rootMenu = std::make_unique<Menu>("cli");
    rootMenu->Insert(
		"set",
		[this](std::ostream& out, std::string key_, std::string value_) {
			SetFancyCommand x(key_, value_);
			std::string response;
			process_cmd_input(x.to_string(), response);
			out << response << "\n";
		},
		"Command to set key to storage",
		{"key", "value"}
    );

    rootMenu->Insert(
		"get",
		[this](std::ostream& out, std::string key_) {
			GetFancyCommand x(key_);
			std::string response;
			process_cmd_input(x.to_string(), response);
			out << response << "\n";
		},
		"Command to get key from storage",
		{"key"}
    );

    Cli cli( std::move(rootMenu), std::make_unique<cli::FileHistoryStorage>(".cli") );

    // global exit action
    cli.ExitAction( [](auto& out){ out << "Goodbye and thanks for all the fish. Remember that you so fancy!.\n"; } );

    // std exception custom handler
    cli.StdExceptionHandler(
            [](std::ostream& out, const std::string& cmd, const std::exception& e)
            {
                out << "Exception caught in CLI handler: "
                    << e.what()
                    << " while handling command: "
                    << cmd
                    << ".\n";
            }
    );
    // // custom handler for unknown commands
    // cli.WrongCommandHandler(
    //         [](std::ostream& out, const std::string& cmd)
    //         {
    //             out << "Unknown command or incorrect parameters: "
    //                 << cmd
    //                 << ".\n";
    //         }
    // );

    boost::asio::io_context ioc;
    cli::LoopScheduler scheduler;
    // cli::BoostAsioScheduler scheduler(ioc);
    cli::CliLocalTerminalSession localSession(cli, scheduler, std::cout, 10);

    localSession.ExitAction(
            [&scheduler](auto& out) // session exit action
            {
                out << "Closing App...\n";
                scheduler.Stop();
            }
    );

    m_out_stream = &localSession.OutStream();

    scheduler.Run();

    m_out_stream = nullptr;

	return true;
}

bool client_t::exit() noexcept
{
	// Send EXIT message
	message_t exit_msg;
	exit_msg.set(message_t::type_t::exit, "EXIT");
	spdlog::info("sending exit message to server [{}]", exit_msg.dump());
	if (!utils::send(m_socket, exit_msg))
	{
		spdlog::error("error while sending command request");
		return false;
	}
	spdlog::info("exit message sent");
	spdlog::info("session completed");
	return true;
}

void client_t::disconnect() noexcept
{
	if (m_socket.is_open())
	{
		m_socket.close();
		spdlog::info("disconnected");
	}
}

void client_t::handle_error(const boost::system::error_code& error)
{
	if (error == boost::asio::error::eof)
	{
		// End of file reached
		spdlog::info("End of file reached, client disconnected");
	}
	else if (error)
	{
		// Some other error occurred
		spdlog::info("Unknown error: {}", error.message());
	}
}