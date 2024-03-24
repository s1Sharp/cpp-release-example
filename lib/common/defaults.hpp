#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

namespace defaults
{
	namespace server
	{
		static constexpr auto ip = "127.0.0.1";
		static constexpr unsigned short port = 9900;
		static constexpr unsigned short threads = 10;

		static constexpr size_t BASE_PING_INTEVAL_SEC = 3;
	}
}

#endif
