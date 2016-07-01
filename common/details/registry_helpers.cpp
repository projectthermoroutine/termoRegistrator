#include <common/registry_helpers.h>
#include <common/fs_helpers.h>
#include <common/log_and_throw.h>

#include <Shlwapi.h>
#include <memory>
#pragma comment(lib, "Shlwapi.lib")

namespace registry_helpers
{
	string_t get_string_from_registry(const HKEY& key, const string_t& subkey, const string_t& value)
	{
		LOG_STACK();

        auto data_size = DWORD{ UNICODE_STRING_MAX_CHARS };
        auto result_name = std::unique_ptr<wchar_t[]>{ new wchar_t[static_cast<std::size_t>(data_size)] };

		const auto call_result = SHGetValueW(key, subkey.c_str(), value.c_str(), nullptr, result_name.get(), &data_size);
		if (call_result == ERROR_SUCCESS)
			return{ result_name.get() };
		else
			LOG_AND_THROW(std::runtime_error("failed_to_read_value_from_registry")) << "error code: " << call_result << " subkey: " << subkey << " value: " << value;
	}

	void set_string_to_registry(const HKEY& hkey, const string_t& key_name, const string_t& value, const string_t& string_data)
	{
		LOG_STACK();

		unsigned long long data_size = (string_data.size() + 1) * 2;//  +1 for null-terminator; *2 for WCHAR
		if (data_size > std::numeric_limits<DWORD>::max())
			LOG_AND_THROW(std::runtime_error("failed_to_set_registry_value")) << "data is too long";

		auto out_size = static_cast<DWORD>(data_size);

		fs_helpers::scoped_thread_wow64_redirect_disable redirect_disable;

		auto key = open_or_create_key(hkey, key_name.c_str());

		const DWORD type = REG_SZ;

		const auto call_result = RegSetValueExW(
			key.get(),
			value.c_str(),
			DWORD(),
			type,
			reinterpret_cast<const BYTE*>(string_data.data()),
			out_size);

		if(call_result != ERROR_SUCCESS)
			LOG_AND_THROW(std::runtime_error("failed_to_set_registry_value")) << "value: " << value << " call result : " << call_result;
	}

	DWORD get_dword_from_registry(const HKEY& key, const string_t& subkey, const string_t& value)
	{
		LOG_STACK();

		auto result = DWORD{};
		auto type = DWORD{};
		auto data_size = static_cast<DWORD>(sizeof(result));

		const auto call_result = SHGetValueW(key, subkey.c_str(), value.c_str(), &type, &result, &data_size);
		if (call_result == ERROR_SUCCESS)
		{
			if (type == REG_DWORD)
				return result;
			else
				LOG_AND_THROW(std::runtime_error("invalid_value_type_of_the_registry_key")) << "Invalid value type " << type << " of the key " << subkey << '\'' << value << ".Expected type : " << REG_DWORD;
		}
		else
		{
			LOG_AND_THROW(std::runtime_error("failed_to_read_value_from_registry")) << "error code : " << call_result << " subkey : " << subkey << " value : " << value;
		}
	} //-V591

	binary_data_t get_binary_from_registry(const HKEY& key, const string_t& subkey, const string_t& value)
	{
		LOG_STACK();
		
		auto type = DWORD{};
		auto data_size = DWORD{0};

		auto call_result = SHGetValueW(key, subkey.c_str(), value.c_str(), &type, nullptr, &data_size);
		if(call_result == ERROR_SUCCESS)
		{
			if(type == REG_BINARY)
			{
				binary_data_t result(static_cast<size_t>(data_size));

				auto call_result = SHGetValueW(key, subkey.c_str(), value.c_str(), &type, result.data(), &data_size);
				if(call_result == ERROR_SUCCESS)
				{
					if(type == REG_BINARY)
						return result;
					else
						LOG_AND_THROW(std::runtime_error("invalid_value_type_of_the_registry_key")) << "Invalid value type " << type << " of the key " << subkey << '\'' << value << ".Expected type : " << REG_DWORD;
				}
				else
					LOG_AND_THROW(std::runtime_error("failed_to_read_value_from_registry")) << "error code : " << call_result << " subkey : " << subkey << " value : " << value;
			}
			else
				LOG_AND_THROW(std::runtime_error("invalid_value_type_of_the_registry_key")) << "Invalid value type " << type << " of the key " << subkey << '\'' << value << ".Expected type : " << REG_DWORD;
		}
		else
			LOG_AND_THROW(std::runtime_error("failed_to_read_value_from_registry")) << "error code : " << call_result << " subkey : " << subkey << " value : " << value;
	}

	void set_binary_to_registry(const HKEY& hkey, const string_t& key_name, const string_t& value, const binary_data_t& binary_data)
	{
		LOG_STACK();

		if(binary_data.size() > std::numeric_limits<DWORD>::max())
			LOG_AND_THROW(std::runtime_error("failed_to_set_registry_value")) << "data is too long";

		fs_helpers::scoped_thread_wow64_redirect_disable redirect_disable;

		auto key = open_or_create_key(hkey, key_name.c_str());

		const DWORD type = REG_BINARY;
		auto out_size = static_cast<DWORD>(binary_data.size());

		const auto call_result = RegSetValueExW(
			key.get(),
			value.c_str(),
			DWORD(),
			type,
			reinterpret_cast<const BYTE*>(binary_data.data()),
			out_size);

		if(call_result != ERROR_SUCCESS)
			LOG_AND_THROW(std::runtime_error("failed_to_set_registry_value")) << "value: " << value << " call result : " << call_result;
	}

	template<typename T>
	typename std::enable_if<std::is_pod<T>::value, T>::type get_pod_from_registry(const HKEY& key, const string_t& subkey, const string_t& value_name)
	{
		LOG_STACK();

		const auto& binary_data = get_binary_from_registry(key, subkey, value_name);
		if(binary_data.size() != sizeof(T))
			LOG_AND_THROW(std::runtime_error("invalid_size_of_value_of_the_registry_key")) << "Invalid size of value of the key "  << subkey << '\'' << value_name << ".Expected size : " << sizeof(T); //-V128

		auto result = T{};
		memcpy(&result, binary_data.data(), binary_data.size());

		return result;
	}

	std::time_t get_time_t_from_registry(const HKEY& key, const string_t& subkey, const string_t& value_name)
	{
		return get_pod_from_registry<std::time_t>(key, subkey, value_name);
	}

	template<typename T>
	typename std::enable_if<std::is_pod<T>::value>::type set_pod_to_registry(const HKEY& key, const string_t& subkey, const string_t& value_name, const T& value)
	{
		LOG_STACK();

		binary_data_t binary_data(sizeof(value));
		memcpy(binary_data.data(), &value, sizeof(value));

		return set_binary_to_registry(key, subkey, value_name, binary_data);
	}

	void set_time_t_to_registry(const HKEY& key, const string_t& subkey, const string_t& value_name, const std::time_t& value)
	{
		LOG_STACK();

		return set_pod_to_registry(key, subkey, value_name, value);
	}

	FILETIME get_filetime_from_registry(const HKEY& key, const string_t& subkey, const string_t& value_name)
	{
		LOG_STACK();

		return get_pod_from_registry<FILETIME>(key, subkey, value_name);
	}

	void set_filetime_to_registry(const HKEY& key, const string_t& subkey, const string_t& value_name, const FILETIME& value)
	{
		LOG_STACK();

		return set_pod_to_registry(key, subkey, value_name, value);
	}

	generic_handle_holder<HKEY> open_or_create_key(HKEY root_key, const wchar_t* key_name)
	{
		//  Windows RegCreateKeyEx WOW64 BUG workaround
		LOG_STACK();

		if (key_name == nullptr)
			LOG_AND_THROW(std::runtime_error("invalid_argument_key_name_was_passed_to_the_open_or_create_key_function"));

		const auto close_key_func = [](const HKEY& key){
			if (RegCloseKey(key) != ERROR_SUCCESS)
			{
				LOG_FATAL() << "Failed to close registry handle. Terminating.";
				std::terminate();
			}
		};

		HKEY hkey = HKEY();

		const auto call_result = RegOpenKeyEx(
			root_key,
			key_name,
			0,
			KEY_WRITE,
			&hkey);

		if (call_result == ERROR_SUCCESS)
		{
			return generic_handle_holder<HKEY>(hkey, close_key_func);
		}

		auto key_name_str = std::wstring{ key_name };
		if (key_name_str.empty())
			LOG_AND_THROW(std::invalid_argument("invalid_argument_key_name_was_passed_to_the_open_or_create_key_function"));

		auto current_block = std::wstring{};
		auto result = generic_handle_holder<HKEY>();
		auto next_delimiter = key_name_str.find(L'\\', current_block.size() + 1);

		if (next_delimiter == key_name_str.npos)
			LOG_AND_THROW(std::invalid_argument("invalid_argument_key_name_was_passed_to_the_open_or_create_key_function"));

		while (next_delimiter != key_name_str.npos)
		{
			next_delimiter = key_name_str.find(L'\\', current_block.size() + 1);
			current_block = key_name_str.substr(0, next_delimiter);
			if (current_block.empty())
				break;

			const auto call_result = RegCreateKeyEx(
				root_key,
				current_block.c_str(),
				DWORD(),
				nullptr,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				nullptr,
				&hkey,
				nullptr);

			if (call_result != ERROR_SUCCESS)
				LOG_AND_THROW(std::runtime_error("failed_to_create_registry_key")) << "key: " << current_block;

			result = generic_handle_holder < HKEY > { hkey, close_key_func };
		};

		return result;
	}

	template<DWORD type_value, typename value_t>
	void set_value_to_registry(const HKEY& hkey, const wchar_t* key_name, const wchar_t* value_name, const value_t& value)
	{
		LOG_STACK();

		fs_helpers::scoped_thread_wow64_redirect_disable redirect_disable;

		auto key = open_or_create_key(hkey, key_name);

		DWORD type = type_value;
		DWORD out_size = static_cast<size_t>(sizeof(value_t));

		const auto call_result = RegSetValueExW(
			key.get(),
			value_name,
			DWORD(),
			type,
			reinterpret_cast<const BYTE*>(&value),
			out_size);

		if (call_result != ERROR_SUCCESS)
			LOG_AND_THROW(std::runtime_error("failed_to_set_registry_value")) << "value: " << value_name << " call result : " << call_result;
	}

	void set_time_t_to_registry(const HKEY& hkey, const wchar_t* key_name, const wchar_t* value_name, const std::time_t& value)
	{
		LOG_STACK();

		return set_value_to_registry<REG_BINARY>(hkey, key_name, value_name, value);
	}

	void set_dword_to_registry(const HKEY& hkey, const wchar_t* key_name, const wchar_t* value_name, const DWORD value)
	{
		LOG_STACK();

		return set_value_to_registry<REG_DWORD>(hkey, key_name, value_name, value);
	}

	void delete_value_from_registry(const HKEY& root_key, const string_t& subkey, const string_t& value)
	{
		LOG_STACK();

		if (root_key == nullptr)
			LOG_AND_THROW(std::invalid_argument("del_value_from_registry_root_param_is_empty"));
		if (subkey.empty())
			LOG_AND_THROW(std::invalid_argument("del_value_from_registry_subkey_param_is_empty"));
		if (value.empty())
			LOG_AND_THROW(std::invalid_argument("del_value_from_registry_value_param_is_empty"));

		LOG_DEBUG() << L"Subkey = '" << subkey << L"', Value = '" << value << L"'.";

		HKEY hkey = HKEY();
		const auto call_result = RegOpenKeyEx(
			root_key,
			subkey.c_str(),
			0,
			KEY_ALL_ACCESS,
			&hkey);

		if (call_result == ERROR_SUCCESS)
		{
			const auto close_key_func = [&subkey, &value](const HKEY& key){
				if (RegCloseKey(key) != ERROR_SUCCESS)
				{
					LOG_AND_THROW(std::runtime_error("Could not close registry handle. RegCloseKey failed.")) << L" Subkey = '" << subkey << L"', Value = '" << value << L"'.";
				}
			};
			generic_handle_holder<HKEY>(hkey, close_key_func);

			if (ERROR_SUCCESS == RegDeleteValue(hkey, value.c_str()))
			{
				LOG_DEBUG() << L"Value '" << value << L"' was deleted from '" << subkey << L"' successfully.";
				return;
			}
			else
			{
				LOG_AND_THROW(std::runtime_error("del_value_from_registry_failed_to_delete_value"));
			}
		}
		else
		{
			LOG_AND_THROW(std::runtime_error("del_value_from_registry_failed_to_open_subkey"));
		}

	}
}