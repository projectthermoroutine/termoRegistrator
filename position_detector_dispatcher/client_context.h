#pragma once

#include <Windows.h>
#include <memory>
#include <vector>
#include <common\shared_memory_channel.h>


namespace position_detector
{

	class client_context_exception : public std::runtime_error
	{
	public:
		client_context_exception(HRESULT error_code, const std::string & message);
		const char * what() const override;
		HRESULT get_error_code() const;

	private:
		std::string _message;
		HRESULT _error_code;
	};


	enum class packet_type{
		synchronization_packet,
		event_packet
	};



	class client_context;

	typedef std::shared_ptr<client_context> client_context_ptr_t;
	typedef std::vector<client_context_ptr_t> clients_context_list_t;

	using namespace channels;
	class client_context final
	{
	public:
		client_context(uint32_t id,packet_type packet_type);
		~client_context();

		std::wstring get_shared_memory_name() const;
		unsigned int get_shared_memory_size() const;
		std::wstring get_event_name() const;

		void send_data(const BYTE * data, unsigned int data_size);

		bool operator==(const client_context& other) const;

		uint32_t ID() const;
	private:
		shared_memory_channel *_channel;
	};

}