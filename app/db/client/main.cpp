#include <boost/program_options.hpp>
#include <iostream>
#include "tcp_client.hpp"

int main(int argc, const char **argv)
{
    uint16_t port_num;
    std::string ip_addr;
    namespace program_options = boost::program_options;
    try
    {
        program_options::options_description desc{"Options"};
        desc.add_options()
            ("help,h", "Help screen")
            ("port", program_options::value<float>()->default_value(3333), "Port")
            ("ip", program_options::value<std::string>()->default_value("127.0.0.1"), "IP Address");

        program_options::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << '\n';
            return 0;
        }
        if (vm.count("ip"))
        {
            ip_addr = vm["ip"].as<std::string>();
            std::cout << "IP: " << vm["ip"].as<std::string>() << '\n';
        }
        if (vm.count("port"))
        {
            port_num = vm["port"].as<float>();
            std::cout << "Port: " << port_num << '\n';
        }
    }
    catch (const program_options::error &ex)
    {
        std::cerr << ex.what() << '\n';
    }

    std::string request;
    do
    {
        std::cout << "Enter request: ";
        std::getline(std::cin >> std::ws, request);
        network::tcp_client tc(ip_addr, port_num);
        tc.send_request(request + "\n");
    } while(request != "q");
}