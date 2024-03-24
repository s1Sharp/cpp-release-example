#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "defaults.hpp"
#include "server.hpp"
#include "db_service.hpp"

void set_logger(bool useConsole)
{
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	// Create a file rotating logger with 5 MB size max and 3 rotated files
	auto max_size = 1024 * 1024 * 5;
	auto max_files = 3;
	std::vector<spdlog::sink_ptr> sinks{};
	if (useConsole) {
		sinks.push_back(console_sink);
	}
	auto rotate_file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/fancy.serv.log", max_size, max_files); // 5 MB, 3 файлов
	sinks.push_back(rotate_file_sink);

	// Добавим файловый синк в дефолтный логгер
	spdlog::default_logger_raw()->sinks() = sinks;

	spdlog::default_logger_raw()->flush_on(spdlog::level::info);
	spdlog::flush_every(std::chrono::seconds(5));

	spdlog::default_logger_raw()->set_level(spdlog::level::debug);
}


int main()
{
	set_logger(true);

	server_t server{defaults::server::ip, defaults::server::port, defaults::server::threads};
	if (!server.start())
	{
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
