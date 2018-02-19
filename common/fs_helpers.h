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
	using stream_enumeration_func_t = std::function<void(const string_t &, file_size_t)>;
	
	stream_data_size_t get_stream_data_size(std::istream & stream);

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