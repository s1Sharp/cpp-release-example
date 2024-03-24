#ifndef DB_SERVICE_HPP
#define DB_SERVICE_HPP

#include <iostream>
#include <string>
#include <sstream>
#include "sw/redis++/redis++.h"

class I_KVDB_service {
public:
    I_KVDB_service(const std::string& url, const std::string& pass) : m_url(url), m_password(pass) {}

    virtual bool set(const std::string& key, const std::string& value) = 0;
    virtual bool get(const std::string& key, std::string return_val) = 0;

    virtual ~I_KVDB_service() {}

protected:
    const std::string m_url;
    const std::string m_password;
};

class RedisDbService : public I_KVDB_service {
public:
    RedisDbService(const std::string& url, const std::string& pass);

    bool set(const std::string& key, const std::string& value) override;

    bool get(const std::string& key, std::string return_val) override;

    ~RedisDbService() override;

private:
    sw::redis::Redis m_redis_client;
};

class DbServiceWrapper {
public:
    DbServiceWrapper(I_KVDB_service* service);

    std::string executeCommand(const std::string& command);

private:
    I_KVDB_service* m_db_service;
};

#endif
