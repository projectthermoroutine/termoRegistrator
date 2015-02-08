#include "CppUnitTest.h"
#include <unit_tests_common.h>

#include <Windows.h>
#include <minifilter_channel.h>
#include <algorithm>
#include <limits>
#include <vector>
#include <string>
#include <xor_shift.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;


namespace
{
	void * memory_alloc(ULONG size)
	{
		return new BYTE[size];
	}
	
	void memory_free(void * p)
	{
		delete[] p;
	}

	template <typename T>
	bool operator != (const T & x, const T & y)
	{
		return !(x == y);
	}

	bool operator == (const WIDE_STRING & x, const WIDE_STRING & y)
	{
		if (x.length != y.length)
		{
			return false;
		}

		if (x.length != 0)
		{
			Assert::IsNotNull(x.str);
		}

		if (y.length != 0)
		{
			Assert::IsNotNull(y.str);
		}

		if (x.length == 0 && y.length == 0)
		{
			return true;
		}

		auto x_ptr = x.str;
		auto y_ptr = y.str;

		for (decltype(x.length) n = 0; n != x.length; ++n, ++x_ptr, ++y_ptr)
		{
			if (*x_ptr != *y_ptr)
			{
				return false;
			}
		}

		return true;
	}

	bool operator == (const MESSAGE_ACKNOWLEDGE & x, const MESSAGE_ACKNOWLEDGE & y)
	{
		return x.message_id == y.message_id;
	}

	bool operator == (const MESSAGE_SCAN_REQUEST & x, const MESSAGE_SCAN_REQUEST & y)
	{
		return x.message_id == y.message_id && x.file_name == y.file_name &&
			x.scan_id == y.scan_id && x.access_token == y.access_token &&
			x.process_id == y.process_id && x.thread_id == y.thread_id &&
			x.thread_priority == y.thread_priority && x.process_priority_class == y.process_priority_class;
	}

	bool operator == (const MESSAGE_SCAN_RESULT & x, const MESSAGE_SCAN_RESULT & y)
	{
		return x.message_id == y.message_id && x.result == y.result && x.scan_id == y.scan_id;
	}

	bool operator == (const MESSAGE_REGISTER_FRIENDLY_PROCESS & x, const MESSAGE_REGISTER_FRIENDLY_PROCESS & y)
	{
		return x.message_id == y.message_id && x.process_id == y.process_id;
	}

	bool operator == (const MESSAGE_UNREGISTER_FRIENDLY_PROCESS & x, const MESSAGE_UNREGISTER_FRIENDLY_PROCESS & y)
	{
		return x.message_id == y.message_id && x.process_id == y.process_id;
	}

	bool operator == (const MESSAGE_NOTIFY_MINIFILTER_UNLOADING & x, const MESSAGE_NOTIFY_MINIFILTER_UNLOADING & y)
	{
		return x.message_id == y.message_id;
	}

	bool operator == (const MESSAGE_PROCESS_FILE & x, const MESSAGE_PROCESS_FILE & y)
	{
		return x.message_id == y.message_id && x.file_name == y.file_name &&
			x.access_token == y.access_token && x.process_id == y.process_id &&
			x.thread_id == y.thread_id && x.process_priority_class == y.process_priority_class &&
			x.thread_priority == y.thread_priority;
	}

	template <typename TNum>
	std::vector<TNum> generate_test_numeric_sequence()
	{
		std::vector<TNum> result;

		result.push_back(std::numeric_limits<MESSAGE_ID>::min());

		for (TNum n = 1; ; n *= 2)
		{
			result.push_back(n);

			const auto new_n = n * 2;
			if (new_n / 2 != n)
			{
				break;
			}
		}

		result.push_back(std::numeric_limits<MESSAGE_ID>::max());

		return result;
	}

	template <typename TMessage, typename TPackFunc, typename TUnpackFunc, typename TUnpackedFreeFunc>
	void test_pack_unpack(
		const TMessage & original_message,
		TPackFunc pack_func,
		TUnpackFunc unpack_func,
		TUnpackedFreeFunc unpacked_free_func,
		MESSAGE_TYPE message_type)
	{
		const MEMORY_MANAGEMENT_FUNCS mem_funcs{ memory_alloc, memory_free };

		Assert::IsTrue(original_message == original_message); //-V501
		Assert::IsFalse(original_message != original_message); //-V501

		auto packed_message = pack_func(&original_message, &mem_funcs);
		Assert::IsNotNull(packed_message);

		Assert::IsTrue(get_message_size(packed_message) != 0);
		Assert::AreEqual(BOOLEAN(TRUE), is_message_well_formed(packed_message));
		Assert::IsTrue(get_message_type(packed_message) == message_type);
		Assert::IsTrue(get_message_id(packed_message) == original_message.message_id);

		auto unpacked_message = unpack_func(packed_message, &mem_funcs);
		Assert::IsNotNull(unpacked_message);

		Assert::IsTrue(*unpacked_message == original_message);

		unpacked_free_func(unpacked_message, mem_funcs.free_func);

		mem_funcs.free_func(packed_message);
	}

	template <
		typename TMessage,
		typename TIter,
		typename TPackFunc,
		typename TUnpackFunc,
		typename TUnpackedFreeFunc>
	void test_pack_unpack(
		TIter src_begin,
		TIter src_end,
		TPackFunc pack_func,
		TUnpackFunc unpack_func,
		TUnpackedFreeFunc unpacked_free_func,
		MESSAGE_TYPE message_type)
	{
		std::for_each(
			src_begin,
			src_end,
			[&pack_func, &unpack_func, &unpacked_free_func, &message_type](const TMessage & message)
			{ test_pack_unpack(message, pack_func, unpack_func, unpacked_free_func, message_type); });
	}

	template <typename T>
	void init_test_value(T & value, std::uint64_t x)
	{
		value = static_cast<T>(std::numeric_limits<T>::max() ^ utils::xor_shift_next(x));
	}
}

namespace common_lib_tests_project
{
	TEST_CLASS(minifilter_channel_test)
	{
	public:
		TEST_METHOD(test_message_parsing_funcs)
		{
			checked_execute([] {				
				const MESSAGE_SIZE_T min_message_size = sizeof(MESSAGE_SIZE_T)+sizeof(MESSAGE_TYPE)+sizeof(MESSAGE_ID);
				
				for (MESSAGE_SIZE_T size = 0; size != min_message_size; ++size)
				{
					Assert::AreEqual(BOOLEAN(FALSE), is_message_well_formed(&size));
					Assert::IsTrue(get_message_size(&size) == size);
				}

				Assert::AreEqual(BOOLEAN(TRUE), is_message_well_formed(&min_message_size));
				Assert::IsTrue(get_message_size(&min_message_size) == min_message_size);
			});
		}

		TEST_METHOD(test_acknowledge)
		{
			checked_execute([] {
				std::vector<MESSAGE_ID> id_seq(generate_test_numeric_sequence<MESSAGE_ID>());
					
				std::vector<MESSAGE_ACKNOWLEDGE> messages;
				for (auto value : id_seq)
				{
					messages.push_back({ value });
				}

				test_pack_unpack<MESSAGE_ACKNOWLEDGE>(
					messages.begin(),
					messages.end(),
					pack_message_acknowledge,
					unpack_message_acknowledge,
					free_unpacked_acknowledge,
					AvMessageAcknowledge);
			});
		}

		TEST_METHOD(test_acknowledge_max_reply_size)
		{
			checked_execute([] {
				MESSAGE_ACKNOWLEDGE message{ 0 };
				const MEMORY_MANAGEMENT_FUNCS mem_funcs{ memory_alloc, memory_free };
				auto packed_message = pack_message_acknowledge(&message, &mem_funcs);

				Assert::IsTrue(get_message_size(packed_message) <= REPLY_MESSAGE_MAX_SIZE);

				memory_free(packed_message);
			});
		}

		TEST_METHOD(test_scan_request)
		{
			checked_execute([] {
				std::vector<MESSAGE_SCAN_REQUEST> messages;

				std::wstring str(L"1234567890abcdef");
				for (unsigned int n = 1; str.length() <= 65536; str += str, ++n)
				{
					std::uint64_t x = n;

					MESSAGE_SCAN_REQUEST message;
					init_test_value(message.message_id, x++);
					init_test_value(message.scan_id, x++);
					init_test_value(message.process_id, x++);
					init_test_value(message.thread_id, x++);
					init_test_value(message.process_priority_class, x++);
					init_test_value(message.thread_priority, x++);
					init_test_value(message.access_token, x++);

					message.file_name.length = str.length();
					message.file_name.str = new WCHAR[message.file_name.length];

					str.copy(message.file_name.str, message.file_name.length);

					messages.push_back(message);
				}

				test_pack_unpack<MESSAGE_SCAN_REQUEST>(
					messages.begin(),
					messages.end(),
					pack_message_scan_request,
					unpack_message_scan_request,
					free_unpacked_scan_request,
					AvMessageScanRequest);

				for (auto message : messages)
				{
					delete[] message.file_name.str;
				}
			});
		}

		TEST_METHOD(test_scan_request_scan_id)
		{
			checked_execute([] {
				const std::vector<SCAN_ID> id_seq(generate_test_numeric_sequence<SCAN_ID>());

				const std::wstring str(L"1234567890abcdef");
				const MESSAGE_ID message_id = std::numeric_limits<MESSAGE_ID>::max();
				const PROCESS_ID process_id = std::numeric_limits<PROCESS_ID>::max();
				const THREAD_ID thread_id = std::numeric_limits<THREAD_ID>::max();
				const PROCESS_PRIORITY_CLASS_T priority_class = std::numeric_limits<PROCESS_PRIORITY_CLASS_T>::max();
				const THREAD_PRIORITY_T priority = std::numeric_limits<THREAD_PRIORITY_T>::max();
				const ACCESS_TOKEN_T token = std::numeric_limits<ACCESS_TOKEN_T>::max();

				std::vector<MESSAGE_SCAN_REQUEST> messages;
				for (auto scan_id : id_seq)
				{
					MESSAGE_SCAN_REQUEST scan_request;
					scan_request.message_id = message_id;
					scan_request.scan_id = scan_id;
					scan_request.access_token = token;
					scan_request.process_id = process_id;
					scan_request.thread_id = thread_id;
					scan_request.process_priority_class = priority_class;
					scan_request.thread_priority = priority;
					scan_request.file_name.str = const_cast<wchar_t*>(str.c_str());
					scan_request.file_name.length = str.length();
					messages.push_back(scan_request);
				}

				test_pack_unpack<MESSAGE_SCAN_REQUEST>(
					messages.begin(),
					messages.end(),
					pack_message_scan_request,
					unpack_message_scan_request,
					free_unpacked_scan_request,
					AvMessageScanRequest);
			});
		}

		TEST_METHOD(test_scan_result)
		{
			checked_execute([] {
				std::vector<MESSAGE_SCAN_RESULT> messages;

				AVSCAN_RESULT_T results[] =
				{
					AvScanResultUndetermined,
					AvScanResultInfected,
					AvScanResultClean
				};

				for (auto result : results)
				{
					MESSAGE_SCAN_RESULT message;
					message.message_id = 0;
					message.result = result;
					
					messages.push_back(message);
				}

				test_pack_unpack<MESSAGE_SCAN_RESULT>(
					messages.begin(),
					messages.end(),
					pack_message_scan_result,
					unpack_message_scan_result,
					free_unpacked_scan_result,
					AvMessageScanResult);
			});
		}

		TEST_METHOD(test_scan_result_scan_id)
		{
			checked_execute([] {
				const std::vector<SCAN_ID> id_seq(generate_test_numeric_sequence<SCAN_ID>());

				const MESSAGE_ID message_id = std::numeric_limits<MESSAGE_ID>::max();
				const AVSCAN_RESULT_T result = AvScanResultInfected;

				std::vector<MESSAGE_SCAN_RESULT> messages;
				for (auto scan_id : id_seq)
				{
					MESSAGE_SCAN_RESULT scan_result;
					scan_result.message_id = message_id;
					scan_result.scan_id = scan_id;
					scan_result.result = result;
					messages.push_back(scan_result);
				}

				test_pack_unpack<MESSAGE_SCAN_RESULT>(
					messages.begin(),
					messages.end(),
					pack_message_scan_result,
					unpack_message_scan_result,
					free_unpacked_scan_result,
					AvMessageScanResult);
			});
		}

		TEST_METHOD(test_register_friendly_process)
		{
			checked_execute([] {
				std::vector<PROCESS_ID> id_seq(generate_test_numeric_sequence<PROCESS_ID>());

				std::vector<MESSAGE_REGISTER_FRIENDLY_PROCESS> messages;
				for (auto value : id_seq)
				{
					MESSAGE_REGISTER_FRIENDLY_PROCESS message;
					message.message_id = 0;
					message.process_id = value;

					messages.push_back(message);
				}

				test_pack_unpack<MESSAGE_REGISTER_FRIENDLY_PROCESS>(
					messages.begin(),
					messages.end(),
					pack_message_register_friendly_process,
					unpack_message_register_friendly_process,
					free_unpacked_register_friendly_process,
					AvMessageRegisterFriendlyProccess);
			});
		}

		TEST_METHOD(test_unregister_friendly_process)
		{
			checked_execute([] {
				std::vector<PROCESS_ID> id_seq(generate_test_numeric_sequence<PROCESS_ID>());

				std::vector<MESSAGE_UNREGISTER_FRIENDLY_PROCESS> messages;
				for (auto value : id_seq)
				{
					MESSAGE_UNREGISTER_FRIENDLY_PROCESS message;
					message.message_id = 0;
					message.process_id = value;

					messages.push_back(message);
				}

				test_pack_unpack<MESSAGE_UNREGISTER_FRIENDLY_PROCESS>(
					messages.begin(),
					messages.end(),
					pack_message_unregister_friendly_process,
					unpack_message_unregister_friendly_process,
					free_unpacked_unregister_friendly_process,
					AvMessageUnregisterFriendlyProccess);
			});
		}

		TEST_METHOD(test_notify_minifilter_unloading)
		{
			checked_execute([] {
				std::vector<MESSAGE_ID> id_seq(generate_test_numeric_sequence<MESSAGE_ID>());

				std::vector<MESSAGE_NOTIFY_MINIFILTER_UNLOADING> messages;
				for (auto value : id_seq)
				{
					messages.push_back({ value });
				}

				test_pack_unpack<MESSAGE_NOTIFY_MINIFILTER_UNLOADING>(
					messages.begin(),
					messages.end(),
					pack_message_notify_minifilter_unloading,
					unpack_message_notify_minifilter_unloading,
					free_unpacked_notify_minifilter_unloading,
					AvMessageNotifyMinifilterUnloading);
			});
		}

		TEST_METHOD(test_process_file)
		{
			checked_execute([] {
				std::vector<MESSAGE_PROCESS_FILE> messages;

				std::wstring str(L"1234567890abcdef");
				for (unsigned int n = 1; str.length() <= 65536; str += str, ++n)
				{
					std::uint64_t x = n;
					
					MESSAGE_PROCESS_FILE message;					
					init_test_value(message.message_id, x++);
					init_test_value(message.access_token, x++);
					init_test_value(message.process_id, x++);
					init_test_value(message.thread_id, x++);
					init_test_value(message.process_priority_class, x++);
					init_test_value(message.thread_priority, x++);

					message.file_name.length = str.length();
					message.file_name.str = new WCHAR[message.file_name.length];

					str.copy(message.file_name.str, message.file_name.length);

					messages.push_back(message);
				}

				test_pack_unpack<MESSAGE_PROCESS_FILE>(
					messages.begin(),
					messages.end(),
					pack_message_process_file,
					unpack_message_process_file,
					free_unpacked_process_file,
					AvMessageProcessFile);

				for (auto message : messages)
				{
					delete[] message.file_name.str;
				}
			});
		}

	};
}