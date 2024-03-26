#include <string_view>

#include <spdlog/spdlog.h>

#include "db_service.hpp"


RedisDbService::RedisDbService(const std::string& url, const std::string& pass) : I_KVDB_service(url, pass), m_redis_client(url) {}

bool RedisDbService::set(const std::string& key, const std::string& value) {
	try {
		m_redis_client.set(key, value);
		return true;
	} catch (const sw::redis::Error& e) {
		std::cerr << "Redis SET error: " << e.what() << std::endl;
		return false;
	}
}

bool RedisDbService::get(const std::string& key, std::string& return_val) {
	try {
		auto val = m_redis_client.get(key);
		if (val) {
			// Dereference val to get the returned value of std::string type.
			return_val = *val;
			return true;
		}   // else key doesn't exist.
		return false;
	} catch (const sw::redis::Error& e) {
		std::cerr << "Redis GET error: " << e.what() << std::endl;
		return false;
	}
}

RedisDbService::~RedisDbService() {}

DbServiceWrapper::DbServiceWrapper(boost::shared_ptr<I_KVDB_service> service) : m_db_service(service) {
	if (!m_db_service) {
		throw std::runtime_error("Pointer to DbService is not initialized");
	}
}

std::string DbServiceWrapper::executeCommand(const std::string& command) {
	std::istringstream iss(command);
	std::string cmd;
	iss >> cmd;

	if (cmd == "set") {
		std::string key, value;
		iss >> key >> value;
		if (m_db_service->set(key, value)) {
			spdlog::debug("Key-value {}:{} pair set successfully.", key, value);
			return "(ok)";
		} else {
			return "(fail)";
		}
	} else if (cmd == "get") {
		std::string key;
		iss >> key;
		std::string value;
		if (m_db_service->get(key, value)) {
			return "\"" + value + "\"";
		}
		return "(not exists)";
	} else {
		return "Unknown command.";
	}
}
