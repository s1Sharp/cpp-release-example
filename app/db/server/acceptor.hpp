#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <boost/asio.hpp>
#include <atomic>

namespace network
{

namespace basio = boost::asio;

/**
 * \brief The Acceptor accepts a port number on which it will listen
 *        for the incoming connection requests.
 */
class acceptor
{
public:
    /**
     * \brief acceptor constructor
     * \param ios instance of 'io_service' class is required by socket constructor.
     * \param port_num port number on which it will listen for the incoming connection
     */
    acceptor(basio::io_service &ios, unsigned short port_num);

    /**
     * \brief Start listening and accept incoming connection requests
     */
    void start();

    /**
     * \brief New asynchronous accept operation is not initiated and the acceptor
     *        object is closed
     */
    void stop();

private:

    /**
     * \brief Constructs an active socket object and initiates the asynchronous
     *        accept operation
     */
    void init_accept();

    /**
     * \brief An instance of the Service class is created and its StartHandling()
     *        method is called, which starts handling the connected client.
     * \param ec
     * \param sock
     */
    void on_accept(
        const boost::system::error_code& ec,
        std::shared_ptr<basio::ip::tcp::socket> sock);

private:
    basio::io_service& io_service_;
    basio::ip::tcp::acceptor acceptor_;
    std::atomic<bool> is_stopped_;
};

}

#endif
