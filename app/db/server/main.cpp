#include "tcp_server.hpp"

#include <boost/program_options.hpp>

#include <iostream>
#include <thread>

namespace
{
constexpr auto DEFAULT_THREAD_POOL_SIZE = 2u;

void start_server(uint16_t port_num)
{
    auto thread_pool_size =
        std::thread::hardware_concurrency() * 2;

    if (thread_pool_size == 0)
    {
        thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
    }
    network::tcp_server srv(port_num, thread_pool_size);
    srv.start();
    std::this_thread::sleep_for(std::chrono::seconds(60));

    srv.stop();

}
}

int main(int argc, const char **argv)
{
    uint16_t port_num;
    namespace program_options = boost::program_options;
    try
    {
        program_options::options_description desc{"Options"};
        desc.add_options()
            ("help,h", "Help screen")
            ("port", program_options::value<uint16_t>()->default_value(3333), "Port Number");

        program_options::variables_map vm;
        program_options::store(program_options::parse_command_line(argc, argv, desc), vm);
        program_options::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
            return 0;
        }
        if (vm.count("port"))
        {
            port_num = vm["port"].as<uint16_t>();
        }
    }
    catch (const program_options::error &ex)
    {
        std::cout << ex.what();
    }

    start_server(port_num);

    return 0;
}