#pragma once

#include <common/handle_holder.h>
#include <windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <cstdint>

namespace registry_helpers
{

	using string_t = std::wstring;

	generic_handle_holder<HKEY> open_or_create_key(HKEY root_key, const wchar_t* key_name);

	std::time_t get_time_t_from_registry(const HKEY& key, const string_t& subkey, const string_t& value_name);
	void set_time_t_to_registry(const HKEY&, const string_t& subkey, const string_t& value_name, const std::time_t& value);

	FILETIME get_filetime_from_registry(const HKEY& key, const string_t& subkey, const string_t& value_name);
	void set_filetime_to_registry(const HKEY&, const string_t& subkey, const string_t& value_name, const FILETIME& value);

	void set_dword_to_registry(const HKEY&, const wchar_t* subkey, const wchar_t* value_name, const DWORD value);

	string_t get_string_from_registry(const HKEY& key, const string_t& subkey, const string_t& value);
	void set_string_to_registry(const HKEY& hkey, const string_t& subkey, const string_t& value, const string_t& string_data);

	DWORD get_dword_from_registry(const HKEY& key, const string_t& subkey, const string_t& value);

	using binary_data_t = std::vector<std::uint8_t>;

	binary_data_t get_binary_from_registry(const HKEY& key, const string_t& subkey, const string_t& value);
	void set_binary_to_registry(const HKEY& key, const string_t& subkey, const string_t& value, const binary_data_t&);

	void delete_value_from_registry(const HKEY& key, const string_t& subkey, const string_t& value);
}