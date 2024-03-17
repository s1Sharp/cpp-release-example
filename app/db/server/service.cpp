#include "service.hpp"

#include <boost/log/trivial.hpp>

namespace network
{

service::service(std::shared_ptr<basio::ip::tcp::socket> sock) :
    tcp_socket_(std::move(sock))
{
}

void service::start_handling()
{
    auto req_callback = [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        on_request_received(ec,
                            bytes_transferred);
    };

    basio::async_read_until(*tcp_socket_.get(),
                            request_,
                            '\n',
                            req_callback);
}

void service::on_request_received(
    const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    if (ec)
    {
        BOOST_LOG_TRIVIAL(error) << "Error code = "
                                 << ec.value()
                                 << ". Message: " << ec.message();

        on_finish();
        return;
    }

    BOOST_LOG_TRIVIAL(info) << "Response Bytes transferred: " << bytes_transferred;

    // Process the request.
    response_ = process_request(request_);

    auto res_callback = [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        on_response_sent(ec, bytes_transferred);
    };

    // Initiate asynchronous write operation.
    basio::async_write(*tcp_socket_.get(),
                       basio::buffer(response_),
                       res_callback);
}

void service::on_response_sent(
    const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    if (ec)
    {
        BOOST_LOG_TRIVIAL(error) << "Error code = "
                                 << ec.value()
                                 << ". Message: " << ec.message();
    }

    BOOST_LOG_TRIVIAL(info) << "Response Bytes transferred: " << bytes_transferred;

    on_finish();
}

void service::on_finish()
{
    delete this;
}

std::string service::process_request(basio::streambuf& request) const
{

    // parse the request.
    std::string str;
    std::istream strm(&request);
    std::getline(strm, str);

    BOOST_LOG_TRIVIAL(info) << "Request received: " << str;

    // return the response message.
    std::stringstream ss;
    ss << "Response:";
    ss << str;
    ss << " back\n";
    return ss.str();
}

}