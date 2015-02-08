#pragma once

#include <rpc_light.h>
#include <Windows.h>

namespace rpc_light
{
	class pipe_server final
	{
	public:
		pipe_server(
			const std::wstring & pipe_name,
			rpc_light::function_call_dispatcher && function_call_dispatcher,
			const SECURITY_ATTRIBUTES * sec_attr = nullptr);

		~pipe_server();

		pipe_server(const pipe_server &) = delete;
		pipe_server & operator = (const pipe_server &) = delete;
	private:
		void * _p_impl;
	};
}