#include "server_base.hpp"
#include <boost/log/trivial.hpp>

namespace network
{

server_base::server_base(uint32_t thread_pool_size) :
    thread_pool_size_(thread_pool_size)
{
    work_.reset(new basio::io_service::work(io_service_));
}

void server_base::start()
{
    if (do_init())
    {
        // Create the thread pool
        for (auto i = 0u; i < thread_pool_size_; ++i)
        {
            auto th = std::make_unique<std::thread>(
                [this]()
                {
                    io_service_.run();
                });

            thread_pool_.push_back(std::move(th));
        }
    }
}

void server_base::stop()
{
    if(do_close())
    {
        io_service_.stop();
        work_.reset(nullptr);
        for (auto &th : thread_pool_)
        {
            th->join();
        }
    }
}

}