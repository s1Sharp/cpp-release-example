#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <boost/asio.hpp>

namespace network
{

namespace basio = boost::asio;

/**
 * \brief service class is intended to handle a single connected client by reading the
 *        request, processing it, and then sending back the response message.
 */
class service
{
public:
    explicit service(std::shared_ptr<basio::ip::tcp::socket> sock);

    /**
     * \brief This method starts handling the client by initiating the asynchronous
     *        reading operation to read the request message from the client.
     */
    void start_handling();

private:
    /**
     * \brief callback for the incoming request.
     * \param ec
     * \param bytes_transferred
     */
    void on_request_received(
        const boost::system::error_code& ec,
        std::size_t bytes_transferred);

    /**
     * \brief callback for the response event. If there is no error
     *        then calls the on_finish method.
     * \param ec
     * \param bytes_transferred
     */
    void on_response_sent(
        const boost::system::error_code& ec,
        std::size_t bytes_transferred);

    /**
     * \brief Clean up the allocated service object.
     */
    void on_finish();

    /**
     * \brief Parse and process the incoming request from client.
     * \param request
     * \return
     */
    std::string process_request(basio::streambuf& request) const;

private:
    std::shared_ptr<basio::ip::tcp::socket> tcp_socket_;
    std::string response_;
    basio::streambuf request_;
};

}

#endif
