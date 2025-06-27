#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Passerine
{
	class PASSER_API Log
	{
	public:
		static void Init();
		
		inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define PS_CORE_TRACE(...) ::Passerine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PS_CORE_INFO(...) ::Passerine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PS_CORE_WARN(...) ::Passerine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PS_CORE_ERROR(...) ::Passerine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PS_CORE_FATAL(...) ::Passerine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define PS_TRACE(...) ::Passerine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PS_INFO(...) ::Passerine::Log::GetClientLogger()->info(__VA_ARGS__)
#define PS_WARN(...) ::Passerine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PS_ERROR(...) ::Passerine::Log::GetClientLogger()->error(__VA_ARGS__)
#define PS_FATAL(...) ::Passerine::Log::GetClientLogger()->fatal(__VA_ARGS__)
