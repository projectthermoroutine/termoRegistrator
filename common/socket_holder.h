#pragma once
#include "handle_holder.h"
#include <WinSock2.h>
class socket_handle_holder final : public generic_handle_holder<SOCKET, INVALID_SOCKET>
{
public:
	socket_handle_holder() : generic_handle_holder(&close_handle) {}
	socket_handle_holder(SOCKET handle) : generic_handle_holder(handle, &close_handle) {}

	socket_handle_holder(socket_handle_holder && x) : generic_handle_holder<SOCKET, INVALID_SOCKET>(std::move(x)) {}

	socket_handle_holder & operator = (socket_handle_holder && x)
	{
		generic_handle_holder<SOCKET, INVALID_SOCKET>::operator=(std::move(x));
		return *this;
	}

	socket_handle_holder(const socket_handle_holder &) = delete;
	socket_handle_holder & operator = (const socket_handle_holder &) = delete;

private:
	static void close_handle(SOCKET handle)
	{
		auto result = ::closesocket(handle);
		if (result == SOCKET_ERROR)
		{
			const auto wsa_result = WSAGetLastError();
			LOG_FATAL() << "Failed to close handle, error: " << std::hex << std::showbase << wsa_result << ". Terminating.";
			std::terminate();
		}
	}
};
