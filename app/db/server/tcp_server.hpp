#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "server_base.hpp"

#include <cstdint>

namespace network
{

class acceptor;
/**
 * \brief This class creates a tcp server.
 */
class tcp_server : public server_base
{
public:
    /**
     * \brief Constructor to create a tcp server
     * \param port_num the port number which server listens
     * \param thread_pool_size
     */
    tcp_server(uint16_t port_num, uint32_t thread_pool_size);

    /**
     * \brief Empty virtual destructor required for std::unique_ptr<acceptor>
     */
    ~tcp_server();

protected:
    /**
     * \brief Instantiate acceptor object and start the acceptor.
     * \return true
     */
    bool do_init() override;

    /**
     * \brief stops the acceptor.
     * \return true
     */
    bool do_close() override;

private:
    uint16_t port_num_;
    std::unique_ptr<acceptor> acc_;
};

}

#endif
