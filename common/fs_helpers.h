#pragma once

#include <string>
#include <functional>
#include <cstdint>
#include <iostream>

namespace fs_helpers
{
	using string_t = std::wstring;
	using file_size_t = std::uint64_t;
	using stream_data_size_t = std::uint64_t;
	using scanning_func_t = std::function<void(const string_t &)>;
	using stream_enumeration_func_t = std::function<void(const string_t &, file_size_t)>;
	
	enum class scan_object_type { unknown, file, directory, reparse_point, file_stream };	

	using for_each_file_stream_func_t = std::function<void(
		const string_t &, scanning_func_t)>;

	using for_each_file_stream_func_ex_t = std::function < void(
		const string_t &, stream_enumeration_func_t)>;

	stream_data_size_t get_stream_data_size(std::istream & stream);


	scan_object_type get_scan_object_type(const string_t & object_name);
		
	void for_all_fs_objects(const string_t & path, scanning_func_t func);

	for_each_file_stream_func_t create_for_each_file_stream_function();
	for_each_file_stream_func_ex_t create_for_each_file_stream_ex_function();

	class scoped_thread_wow64_redirect_disable final
	{
	public:
		scoped_thread_wow64_redirect_disable();
		~scoped_thread_wow64_redirect_disable();

		scoped_thread_wow64_redirect_disable(const scoped_thread_wow64_redirect_disable &) = delete;
		scoped_thread_wow64_redirect_disable & operator = (const scoped_thread_wow64_redirect_disable &) = delete;
	private:
		void * old_value;
		bool _disabled;
	};
}