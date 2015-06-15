#pragma once

#define NOMINMAX

//#include <Windows.h>

#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <common\handle_holder.h>

namespace position_detector
{
	class position_detector_connector_exception : public std::runtime_error
	{
	public:
		position_detector_connector_exception(HRESULT error_code, const std::string & message);
		const char * what() const override;
		HRESULT get_error_code() const;
	private:
		std::string _message;
		HRESULT _error_code;
	};

#define DEFAULT_DETECTOR_PACKET_SIZE 4096UL

	using packet_size_t = ULONG32;

#pragma pack (push)
#pragma pack (1)

	struct get_message_struct
	{
		BYTE data[DEFAULT_DETECTOR_PACKET_SIZE];
	};

#pragma pack (pop)

	class connector_api
	{
	public:
		using settings_func_t = std::function<std::string(const std::string &)>;

		connector_api() {}
		virtual ~connector_api() {};
		virtual int get_message(get_message_struct * const buffer, const packet_size_t buffer_size, const HANDLE stop_event) = 0;
		virtual void close() = 0;

		connector_api(const connector_api &) = delete;
		connector_api & operator = (const connector_api &) = delete;
	};

	using connector_api_ptr_t = std::shared_ptr<connector_api>;


	class position_detector_connector final
	{
	public:
		using byte_vector = std::vector<BYTE>;
		using message_processing_func_t = std::function<void (const BYTE *,unsigned int)>;
		using stop_requested_func_t = std::function<bool(void)>;

		position_detector_connector(connector_api_ptr_t api);
		~position_detector_connector();

		void process_incoming_message(
			const stop_requested_func_t& stop_requested,
			const HANDLE stop_event,
			const message_processing_func_t& message_process_func);

		void process_incoming_message(
			const HANDLE stop_event,
			const message_processing_func_t& message_process_func);


		void close();

		position_detector_connector(const position_detector_connector &) = delete;
		position_detector_connector & operator = (const position_detector_connector &) = delete;
	private:
		static packet_size_t get_packet_size(const BYTE * data);

		unsigned int get_message(
			get_message_struct * const buffer,
			const packet_size_t buffer_size,
			const HANDLE stop_event);

		unsigned int get_message(
			get_message_struct * const buffer,
			const packet_size_t buffer_size,
			const stop_requested_func_t& stop_requested,
			const HANDLE stop_event
			);


		std::unique_ptr<get_message_struct> _buffer;
		volatile bool _close_requested;
		connector_api_ptr_t _api;
	};
}