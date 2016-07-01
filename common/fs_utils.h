/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 07.10.2014
// Краткое описание: интерфейс функций для работы с ФС
/////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <common\handle_holder.h>
#include <utility>

namespace fs_utils
{
	using string_t = std::wstring;

	enum hash_type: std::size_t { none, sha256 };

	using hash_container_t = std::vector < std::uint8_t > ;
	using file_size_t = std::int64_t;
	using position_in_file_t = std::int64_t;

	using file_name_and_size = std::pair < string_t, file_size_t >;
	using file_attributes_dword_t = std::uint32_t;

	enum class io_status: std::uint32_t { created = 0, opened = 1, overwritten = 2, superseded = 3, exists = 4, does_not_exist = 5 };
	using io_status_block_t = std::vector<io_status>;

	using create_file_func_t = std::function<handle_holder(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)>;
	using file_operation_func_t = std::function<void(const create_file_func_t& func)>;

	file_size_t get_file_size(const handle_holder& file_handle);
	file_size_t get_file_size(const string_t& file_name);
	
	bool file_exists(const string_t&);
	bool directory_exists(const string_t& directory_name);
	void rename_file(const string_t& src_file, const string_t& dst_file, bool fail_if_dst_exists = true);
	void rename_file(const handle_holder& file_handle, const string_t& new_file_name, bool fail_if_dst_exists = true);
	void xor_first_file_block(const string_t&);
	void secure_erase_file(const string_t&);
	void copy_file(const string_t& src_file, const string_t& dst_file, bool delete_on_close = false);
	io_status_block_t copy_file_ex(const string_t& src_file, const string_t& dst_file, bool delete_on_close = false);
	bool was_file_overwritten(const io_status_block_t&);
	void copy_file(const string_t& src_file, const string_t& dst_file, const file_size_t block_size, bool delete_on_close = false);
	void copy_and_xor_file(const string_t& src_file, const string_t& dst_file, bool delete_on_close = false);
	void copy_xor_and_hash_file(const string_t& src_file, const string_t& dst_file, const hash_type& hash_type, hash_container_t& hash, bool delete_on_close = false, bool hash_before_xor = true);
	std::vector<file_name_and_size> get_file_data_streams(const string_t&);
	void delete_file(const string_t&);
	void hash_xored_file(const string_t&, const hash_type& hash_type, hash_container_t& hash);
	bool compare_files(const string_t& file1, const string_t& file2);
	bool check_access_rights(const string_t& path, DWORD generic_access_rights);

	void remove_read_only_attribute(const string_t& file_name);
	void set_compression_attribute(const string_t& file_name);
	void set_file_attributes(const string_t& file_path, file_attributes_dword_t);
	file_attributes_dword_t get_file_attributes(const string_t& file_path);

	bool compare_file_names(string_t file_name1, string_t file_name2);
	std::vector<std::uint8_t> read_file(const std::wstring& file_name, file_size_t max_file_size);
	std::vector<std::uint8_t> read_file(const handle_holder& handle, file_size_t block_size);
	void write_file(const std::wstring& file_name, const std::vector<std::uint8_t>& data);
	void write_file(const handle_holder& handle, const void* buffer, file_size_t total_size);
	void set_end_of_file(const handle_holder& handle);	

	void set_file_pointer(const handle_holder& handle, position_in_file_t position);
	position_in_file_t get_file_pointer(const handle_holder& handle);

	string_t get_full_path_name(const string_t& short_path);
	bool does_path_contain_resparse_points(const string_t& file_name);

	string_t get_temp_path();
	string_t create_unique_temp_file(const string_t& temp_path, const wchar_t* prefix = L"");

	void create_directory(const string_t& path, const SECURITY_ATTRIBUTES* security_attributes = nullptr, bool fail_if_exists = true);
	void remove_directory(const string_t& path);

	void perform_transaction_on_file(const file_operation_func_t&);

	FILETIME get_file_last_write_time(const string_t & path);

}