#include <cstdlib>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "utils.hpp"
#include "defaults.hpp"
#include "client.hpp"

void set_logger(bool useConsole)
{
    // Очистим все существующие синки из дефолтного логгера
    spdlog::default_logger_raw()->sinks().clear();
    if (useConsole) {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        spdlog::default_logger_raw()->sinks().push_back(console_sink);
    }
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log/fancy.cli.log", false);
	spdlog::default_logger_raw()->sinks().push_back(file_sink);

    spdlog::default_logger_raw()->flush_on(spdlog::level::info);
    spdlog::flush_every(std::chrono::seconds(3));

    spdlog::default_logger_raw()->set_level(spdlog::level::debug);
}

int main()
{
    set_logger(false);

	client_t client{defaults::server::ip, defaults::server::port};
    try {
        if (!client.start()) {
            return EXIT_FAILURE;
        }
    } catch (const utils::BrokenPipeSocketException& bpe) {
        std::cerr << "Error: " << bpe.what() << std::endl;
        std::cerr << "Shutdown client" << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
		return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
