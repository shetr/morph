
#ifndef MORPH_LOG_HPP
#define MORPH_LOG_HPP

#include "Types.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Morph {

	class Log
	{
	public:
		static void Init();

		static shared<spdlog::logger>& engine() { return s_EngineLogger; }
		static shared<spdlog::logger>& client() { return s_ClientLogger; }
	private:
		static shared<spdlog::logger> s_EngineLogger;
		static shared<spdlog::logger> s_ClientLogger;
	};

}

#endif // MORPH_LOG_HPP