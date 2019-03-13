#pragma once

#include <windows.h>
#include <functional>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <future>
#include <common\handle_holder.h>
//#include <error_lib\application_exception.h>

namespace security
{
	using filename_t = std::wstring;

	using func_uses_sec_attr_t = std::function<void(const SECURITY_ATTRIBUTES &)>;
	void process_sec_attributes_for_internal_ipc(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions);
	void process_sec_attributes_for_internal_pipe(func_uses_sec_attr_t func_uses_sec_attr);
	void process_sec_attributes_for_users_pipe(func_uses_sec_attr_t func_uses_sec_attr);
	void process_sec_attributes_for_internal_event_sync(func_uses_sec_attr_t func_uses_sec_attr);
	void process_sec_attributes_for_network_service_event_sync(func_uses_sec_attr_t func_uses_sec_attr);
	void process_file_sec_attributes_for_scan_service(func_uses_sec_attr_t func_uses_sec_attr);
	void process_directory_sec_attributes_for_scan_service(func_uses_sec_attr_t func_uses_sec_attr);
	void process_fs_object_sec_attributes_for_scan_service_and_network_service(func_uses_sec_attr_t func_uses_sec_attr);
	void process_file_sec_attributes_for_scan_worker_temp_directory(func_uses_sec_attr_t func_uses_sec_attr);
	void process_sec_attributes_for_internal_event(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions);

	void process_sec_attributes_for_shared_memory(func_uses_sec_attr_t func_uses_sec_attr);
	void process_sec_attributes_for_shared_memory_event(func_uses_sec_attr_t func_uses_sec_attr, DWORD permissions);

	using session_id_t = std::uint32_t;

	class token_information final
	{
	public:
		explicit token_information(HANDLE token);
		explicit token_information(const handle_holder& token);

		token_information(const token_information &);
		token_information & operator = (const token_information &);

		token_information(token_information &&) noexcept;
		token_information & operator = (token_information &&);

		void swap(token_information & other) noexcept;

	public:

		session_id_t get_session_id() const;
		std::wstring get_sid_str() const;

#ifdef _DEBUG
		std::wstring get_privileges() const;
		std::wstring get_elevated() const;
#endif // _DEBUG

	private:

		static session_id_t build_session_id(HANDLE token);
		static std::wstring build_sid(HANDLE token);

#ifdef _DEBUG
		static std::wstring build_privileges(HANDLE token);
		static std::wstring build_elevated(HANDLE token);
#endif // _DEBUG

	private:
		session_id_t _session_id;
		std::wstring _sid_str;

#ifdef _DEBUG
		std::wstring _privileges;
		std::wstring _elevated;
#endif // _DEBUG
	};

	using sid_string_t = std::wstring;
	using process_id_t = std::uint32_t;

	PSID get_sid_from_str(const sid_string_t & sid_str);
	PSID get_current_process_user_sid();
	handle_holder get_process_token();
	handle_holder get_process_token(HANDLE process, DWORD desired_access = TOKEN_QUERY);
	handle_holder get_process_token(process_id_t process_id);

	handle_holder get_current_thread_token(DWORD desired_access = TOKEN_QUERY | TOKEN_IMPERSONATE);

	handle_holder get_network_service_primary_token();

	std::vector<WELL_KNOWN_SID_TYPE> get_privileged_sids();
	std::vector<WELL_KNOWN_SID_TYPE> get_network_service_and_local_service_ipc_sid();
	bool check_current_thread_membership_in_any_group(const std::vector<WELL_KNOWN_SID_TYPE>& groups);
	bool check_process_membership_in_any_group(security::process_id_t pid, const std::vector<WELL_KNOWN_SID_TYPE>& groups);	
	bool check_token_membership_in_any_group(const handle_holder & token, const std::vector<WELL_KNOWN_SID_TYPE>& groups);
	bool check_token_service_or_admin(const handle_holder & token);

	bool check_current_process_token_is_admin();
	bool check_current_thread_token_is_local_system();

	std::wstring get_current_process_sid_string();
	session_id_t get_current_process_session_id();
	std::tuple<sid_string_t, session_id_t> get_current_process_sid_and_session_id();
	std::wstring get_process_name(const process_id_t process_id);

	std::wstring get_module_file_name(HINSTANCE instance);
	std::wstring get_current_module_path();

	using user_name_t = std::wstring;
	using domain_name_t = std::wstring;
	std::tuple<user_name_t, domain_name_t> get_user_name_and_domain_name(const sid_string_t &);
	
	class scoped_impersonation final
	{
	public:
		scoped_impersonation();
		scoped_impersonation(const handle_holder & access_token);
		~scoped_impersonation();

		scoped_impersonation(const scoped_impersonation &) = delete;
		scoped_impersonation & operator = (const scoped_impersonation &) = delete;

	public:

		std::wstring user_sid() const;

	private:
		std::wstring m_user_sid;
	};

	using execute_for_impersonated_token_func_t    = std::function < void() > ;
	using execute_for_self_token_func_t            = std::function < void() > ;
	using execute_for_impersonated_token_func_ex_t = std::function < void(const std::wstring& user_sid) > ;
	using execute_for_self_token_func_ex_t         = std::function < void(const std::exception_ptr&) > ;

	void try_execute_impersonated(const handle_holder& token, const execute_for_impersonated_token_func_t&, const execute_for_self_token_func_t& = {});
	void try_execute_impersonated_ex(const handle_holder& token, const execute_for_impersonated_token_func_ex_t&, const execute_for_self_token_func_ex_t& = {});

	handle_holder get_adjusted_token(const handle_holder & token, const TOKEN_PRIVILEGES & privileges);

	template <typename func_t, typename... Args>
	auto execute_under_current_process_token(const func_t func, Args... args) -> typename std::result_of<decltype(func)(Args...)>::type
	{
		auto task_under_process_token = std::async(std::launch::async, func, args...);
		return task_under_process_token.get();
	}

	LUID get_luid_from_token(const handle_holder & token);

} // namespace security
