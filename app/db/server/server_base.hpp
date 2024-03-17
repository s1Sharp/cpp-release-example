#ifndef SERVER_BASE_HPP
#define SERVER_BASE_HPP

#include <cstdint>
#include <thread>
#include <vector>
#include <boost/asio.hpp>

namespace network
{

namespace basio = boost::asio;

/**
 * \brief Base class for creating different server types.
 */
class server_base
{
public:
    /**
     * \brief Constructor for server_base.
     * \param thread_pool_size size of the thread pool
     */
    explicit server_base(uint32_t thread_pool_size);

    /**
     * \brief virtual empty base class destructor to avoid memory leaks.
     */
    virtual ~server_base() = default;

    /**
     * \brief This method starts up the server.
     */
    virtual void start();

    /**
     * \brief This method stops the server wait for all the threads to join.
     */
    virtual void stop();

protected:
    /**
     * \brief This method should be overridden in the derived class to instantiate the resources
     *        required for the server
     * type.
     * \return true if instantiated correctly else false.
     */
    virtual bool do_init() = 0;

    /**
     * \brief This method should be overridden in the derived class to free up the resources
     *        instantiated in the do_init() method.
     * \return true if resources are freed up correctly else false.
     */
    virtual bool do_close() = 0;

    uint32_t thread_pool_size_;
    std::vector<std::unique_ptr<std::thread>> thread_pool_;
    basio::io_service io_service_;
    std::unique_ptr<basio::io_service::work> work_;
};

}

#endif