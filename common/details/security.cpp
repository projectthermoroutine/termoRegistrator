
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <functional>
#include <utility>
#include <vector>

#include <loglib\log.h>
#include <common\on_exit.h>
#include <common\security.h>
#include <common\log_and_throw.h>
#include <common\fs_helpers.h>
#include <error_lib\win32_error.h>
#include <error_lib\win32_error_codes.h>
#include <error_lib\application_exception.h>
#include <error_lib\error_codes.h>

#include <Windows.h>
#include <Aclapi.h>
#include <AccCtrl.h>
#include <Sddl.h>
#include <Psapi.h>

#include <atlsecurity.h>

namespace security
{
	namespace token_info
	{

// Этим макросом оборачиваются структуры данных для получения которых ТОЧНО не выделяется больше памяти чем их собственный размер!
#define SIMPLE_CLASS(_Name) _Name; static const bool is_simple = true

		template <TOKEN_INFORMATION_CLASS class_id> struct tkn_class {};
		template <> struct tkn_class<TokenUser>              { using type = TOKEN_USER;                         };
		template <> struct tkn_class<TokenSessionId>         { using type = SIMPLE_CLASS(DWORD);                };
		template <> struct tkn_class<TokenElevation>         { using type = SIMPLE_CLASS(TOKEN_ELEVATION);      };
		template <> struct tkn_class<TokenPrivileges>        { using type = TOKEN_PRIVILEGES;                   };
		template <> struct tkn_class<TokenLinkedToken>       { using type = SIMPLE_CLASS(TOKEN_LINKED_TOKEN);   };
		template <> struct tkn_class<TokenElevationType>     { using type = SIMPLE_CLASS(TOKEN_ELEVATION_TYPE); };
		template <> struct tkn_class<TokenAccessInformation> { using type = TOKEN_ACCESS_INFORMATION;           };

#undef SIMPLE_CLASS

		template <TOKEN_INFORMATION_CLASS class_id>
		using class_t = typename tkn_class<class_id>::type;

		template <TOKEN_INFORMATION_CLASS class_id>
		using class_ptr = std::unique_ptr<class_t<class_id>, std::function<void(class_t<class_id>*)>>;

		template <
			TOKEN_INFORMATION_CLASS class_id,
			std::enable_if_t<tkn_class<class_id>::is_simple>* = nullptr
		>
		void fill_instance(HANDLE handle, class_t<class_id>& instance)
		{
			LOG_STACK();

			if (handle == HANDLE{})
				LOG_AND_THROW(::common::application_exception(::common::result_code::invalid_token_was_passed_to_get_user_info_from_token));

			DWORD buffer_size_required = 0;
			if (!::GetTokenInformation(handle, class_id, &instance, sizeof(instance), &buffer_size_required))
				LOG_AND_THROW(::win32::exception::by_last_error("GetTokenInformation"));

			if (buffer_size_required != sizeof(instance))
				LOG_AND_THROW(::common::application_exception(::common::result_code::gettokeninformation_returned_unexpected_result));
		}

		template <TOKEN_INFORMATION_CLASS class_id>
		class_t<class_id> get_instance(HANDLE handle)
		{
			LOG_STACK();

			class_t<class_id> instance{};

			fill_instance<class_id>(handle, instance);

			return instance;
		}

		template <TOKEN_INFORMATION_CLASS class_id>
		class_ptr<class_id> get_instance_ptr(HANDLE handle)
		{
			LOG_STACK();

			if (handle == HANDLE{})
				LOG_AND_THROW(::common::application_exception(::common::result_code::invalid_token_was_passed_to_get_user_info_from_token));

			DWORD buffer_size_required = 0;
			if (::GetTokenInformation(handle, class_id, nullptr, 0, &buffer_size_required))
				LOG_AND_THROW(::common::application_exception(::common::result_code::gettokeninformation_returned_unexpected_result));

			if (ERROR_INSUFFICIENT_BUFFER != ::GetLastError())
				LOG_AND_THROW(win32::exception::by_last_error("GetTokenInformation"));

			if (buffer_size_required < sizeof(class_t<class_id>))
				LOG_AND_THROW(::common::application_exception(::common::result_code::gettokeninformation_returned_unexpected_result));

			class_ptr<class_id> instance_ptr(
				reinterpret_cast<class_t<class_id>*>(new std::uint8_t[buffer_size_required]),
				[](class_t<class_id>* instance_ptr) { delete[] reinterpret_cast<std::uint8_t*>(instance_ptr); }
			);

			if (!::GetTokenInformation(handle, class_id, instance_ptr.get(), buffer_size_required, &buffer_size_required))
				LOG_AND_THROW(win32::exception::by_last_error("GetTokenInformation"));

			return instance_ptr;
		}

	} // namespace token_info

	namespace details
	{
		using sid_ptr = std::unique_ptr < SID, decltype(&LocalFree) > ;
		using acl_ptr = std::unique_ptr < ACL, decltype(&LocalFree) > ;
		using sd_ptr = std::unique_ptr < SECURITY_DESCRIPTOR, decltype(&LocalFree) > ;
		using func_uses_sd_t = std::function < void(PSECURITY_DESCRIPTOR) > ;

		using namespace ::common;

		sid_ptr create_well_known_sid(WELL_KNOWN_SID_TYPE sid_type)
		{
			LOG_STACK();

			DWORD sid_size = SECURITY_MAX_SID_SIZE;
			sid_ptr p_sid(
				reinterpret_cast<SID*>(LocalAlloc(LMEM_FIXED, sid_size)),
				&LocalFree);

			if (p_sid == nullptr)
				LOG_AND_THROW(win32::exception::by_last_error("LocalAlloc")) << "Could not allocate memory for SID";

			if (!CreateWellKnownSid(sid_type, nullptr, p_sid.get(), &sid_size))
				LOG_AND_THROW(win32::exception::by_last_error("CreateWellKnownSid")) << "Could not create well known SID";

			return p_sid;
		}

		sid_ptr initialize_access_entry(
			EXPLICIT_ACCESSW & ea,
			WELL_KNOWN_SID_TYPE sid_type,
			DWORD access_permissions)
		{
			LOG_STACK();

			ZeroMemory(&ea, sizeof(EXPLICIT_ACCESSW));

			auto p_sid = create_well_known_sid(sid_type);

			ea.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
			ea.grfAccessMode = SET_ACCESS;
			ea.grfAccessPermissions = access_permissions;
			ea.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
			ea.Trustee.pMultipleTrustee = nullptr;
			ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea.Trustee.ptstrName = reinterpret_cast<decltype(ea.Trustee.ptstrName)>(p_sid.get());

			return p_sid;
		}

		std::vector<WELL_KNOWN_SID_TYPE> users_buildin_sids()
		{
			return{WinBuiltinUsersSid};
		}

		std::vector<WELL_KNOWN_SID_TYPE> iis_users_buildin_sids()
		{
			return{WinBuiltinIUsersSid};
		}

		std::vector<WELL_KNOWN_SID_TYPE> users_ipc_sids()
		{
			auto result = get_privileged_sids();
			result.push_back(WinAuthenticatedUserSid);
			return result;
		}

		struct ace_params
		{
			ace_params(DWORD access_permissions, const std::vector<WELL_KNOWN_SID_TYPE>& sids_):sids(sids_)
			{
				this->access_permissions = access_permissions;
			}
			//ace_params(const ace_params& other) :sids(other.sids), access_permissions(other.access_permissions) {}

			//ace_params& operator=(const ace_params& other) = delete;

			DWORD access_permissions;
			std::vector<WELL_KNOWN_SID_TYPE> sids;
		};


		void process_sd_for_internal_ipc(func_uses_sd_t func_uses_sd, const std::vector<ace_params>& ace_params)
		{
			LOG_STACK();

			using namespace details;
			ULONG number_of_all_entries = 0;
			std::vector<sid_ptr> access_entries;
			auto explicit_access = std::vector<EXPLICIT_ACCESSW>();

			for (const auto & ace_param : ace_params)
			{
				const auto number_of_entries = ace_param.sids.size();
				explicit_access.resize(number_of_entries + number_of_all_entries);
				for (size_t i = number_of_all_entries, sid_indx = 0; sid_indx != number_of_entries; ++i, ++sid_indx)
					access_entries.push_back(initialize_access_entry(explicit_access[i], ace_param.sids[sid_indx], ace_param.access_permissions));

				number_of_all_entries += static_cast<ULONG>(number_of_entries);
			}
			acl_ptr p_acl(nullptr, &LocalFree);

			{
				PACL p_acl_raw = nullptr;
				const auto result_code = SetEntriesInAclW(
					number_of_all_entries,
					explicit_access.data(),
					nullptr,
					&p_acl_raw);

				if (result_code != ERROR_SUCCESS)
					LOG_AND_THROW(application_exception(result_code::could_not_set_acl)) << "SetEntriesInAclW failed: " << std::hex << std::showbase << result_code;

				p_acl.reset(p_acl_raw);
			}

			sd_ptr p_sd(
				reinterpret_cast<SECURITY_DESCRIPTOR*>(LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH)),
				&LocalFree);

			if (!InitializeSecurityDescriptor(p_sd.get(), SECURITY_DESCRIPTOR_REVISION))
				LOG_AND_THROW(win32::exception::by_last_error("InitializeSecurityDescriptor")) << "Could not initialize security descriptor";

			if (!SetSecurityDescriptorDacl(p_sd.get(), TRUE, p_acl.get(), FALSE))
				LOG_AND_THROW(win32::exception::by_last_error("SetSecurityDescriptorDacl")) << "Could not set security descriptor's DACL";

			func_uses_sd(p_sd.get());
		}

		void check_sid_and_attributes(const TOKEN_USER& tkn_user_data)
		{
			if (tkn_user_data.User.Sid == nullptr)
				LOG_AND_THROW(application_exception(result_code::null_sid_was_passed));

			if (!IsValidSid(tkn_user_data.User.Sid))
				LOG_AND_THROW(application_exception(result_code::invalid_sid_was_passed));

			if (GetLengthSid(tkn_user_data.User.Sid) == 0)
				LOG_AND_THROW(application_exception(result_code::empty_sid_was_passed));
		}


		using sec_attribute = std::pair < DWORD, std::vector<WELL_KNOWN_SID_TYPE>(*)() > ;

		void process_sec_attributes(func_uses_sec_attr_t func_uses_sec_attr, const std::vector<sec_attribute>& sec_attributes)
		{
			LOG_STACK();

			const auto process_func = [func_uses_sec_attr](PSECURITY_DESCRIPTOR p_sd)
			{
				SECURITY_ATTRIBUTES sec_attr{};
				sec_attr.nLength = sizeof(sec_attr);
				sec_attr.bInheritHandle = FALSE;
				sec_attr.lpSecurityDescriptor = p_sd;
				func_uses_sec_attr(sec_attr);
			};

			std::vector<details::ace_params> ace_params_list;
			for (const auto & attribute : sec_attributes)
			{
				ace_params_list.push_back({attribute.first, attribute.second()});
			}

			details::process_sd_for_internal_ipc(process_func, ace_params_list);
		}

	} // namespace details

	using namespace ::common;

	void process_sec_attributes_for_internal_ipc(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions)
	{
		details::process_sec_attributes(func_uses_sec_attr, {{access_permissions, get_privileged_sids}});
	}

	void process_sec_attributes_for_internal_pipe(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		return process_sec_attributes_for_internal_ipc(func_uses_sec_attr, GENERIC_READ | GENERIC_WRITE);
	}

	void process_sec_attributes_for_internal_event(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions)
	{
		LOG_STACK();
		return process_sec_attributes_for_internal_ipc(func_uses_sec_attr, access_permissions);
	}

	void process_sec_attributes_for_internal_event_sync(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		return process_sec_attributes_for_internal_ipc(func_uses_sec_attr, SYNCHRONIZE);
	}

	void process_sec_attributes_for_network_service_event_sync(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		details::process_sec_attributes(func_uses_sec_attr, {{EVENT_MODIFY_STATE | SYNCHRONIZE, get_network_service_and_local_service_ipc_sid}});
	}

	void process_directory_sec_attributes_for_scan_service(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();

		details::process_sec_attributes(func_uses_sec_attr, {{FILE_ALL_ACCESS, get_privileged_sids}, {GENERIC_READ, details::users_buildin_sids}});
	}

	void process_fs_object_sec_attributes_for_scan_service_and_network_service(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();

		details::process_sec_attributes(func_uses_sec_attr, {
				{FILE_ALL_ACCESS, get_network_service_and_local_service_ipc_sid},
				{GENERIC_READ, details::users_buildin_sids},
				{GENERIC_READ, details::iis_users_buildin_sids}
		});
	}

	void process_file_sec_attributes_for_scan_service(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();

		details::process_sec_attributes(func_uses_sec_attr, {{FILE_ALL_ACCESS, get_privileged_sids}, {GENERIC_READ, details::users_buildin_sids}});
	}

	void process_file_sec_attributes_for_scan_worker_temp_directory(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();

		details::process_sec_attributes(func_uses_sec_attr,
		{
			{
				FILE_ALL_ACCESS,
				get_privileged_sids
			},
			{
				FILE_ALL_ACCESS,
				[]()->std::vector < WELL_KNOWN_SID_TYPE > { return{WinLocalSystemSid, WinCreatorOwnerSid}; }
			},
			{
				FILE_GENERIC_READ | FILE_GENERIC_WRITE | FILE_EXECUTE,
				[]()->std::vector < WELL_KNOWN_SID_TYPE > { return{WinAuthenticatedUserSid}; }
			}
		});
	}

	void process_sec_attributes_for_users_ipc(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions)
	{
		LOG_STACK();
		details::process_sec_attributes(func_uses_sec_attr, {{access_permissions, details::users_ipc_sids}});
	}

	void process_sec_attributes_for_users_pipe(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		return process_sec_attributes_for_users_ipc(func_uses_sec_attr, GENERIC_READ | GENERIC_WRITE);
	}

	void process_sec_attributes_for_shared_memory_event(func_uses_sec_attr_t func_uses_sec_attr, DWORD permissions)
	{
		LOG_STACK();
		details::process_sec_attributes(func_uses_sec_attr, 
		{
			{
				permissions,
				get_network_service_and_local_service_ipc_sid
			},
			{
				permissions,
				[]() -> std::vector< WELL_KNOWN_SID_TYPE > { return{ WinBuiltinUsersSid }; }
			}
		});
	}

	void process_sec_attributes_for_shared_memory(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		details::process_sec_attributes(func_uses_sec_attr, 
		{ 
			{
				FILE_MAP_ALL_ACCESS,
				get_network_service_and_local_service_ipc_sid
			},
			{
				FILE_MAP_ALL_ACCESS,
				[]() -> std::vector< WELL_KNOWN_SID_TYPE > { return{ WinBuiltinUsersSid }; }
			}
		});
	}

	std::wstring convert_sid_to_string(const PSID p_sid)
	{
		LOG_STACK();

		if (p_sid == nullptr)
			LOG_AND_THROW(application_exception(result_code::passed_argument_p_sid_is_null));

		LPWSTR sid_str_buf = nullptr;
		if (!ConvertSidToStringSidW(p_sid, &sid_str_buf))
			LOG_AND_THROW(win32::exception::by_last_error("ConvertSidToStringSid"));

		if (sid_str_buf == nullptr)
		{
			LOG_AND_THROW(application_exception(result_code::convertsidtostringsid_returned_unexpected_result));
		}

		utils::on_exit sid_str_exit_guard([sid_str_buf]
		{
			if (LocalFree(sid_str_buf) != nullptr)
				LOG_AND_THROW(win32::exception::by_last_error("LocalFree"));
		});

		return sid_str_buf;
	}

	session_id_t token_information::build_session_id(HANDLE token)
	{
		return session_id_t(token_info::get_instance<TokenSessionId>(token));
	}

	std::wstring token_information::build_sid(HANDLE token)
	{
		const token_info::class_ptr<TokenUser> user_data(token_info::get_instance_ptr<TokenUser>(token));

		details::check_sid_and_attributes(*user_data.get());

		return convert_sid_to_string(user_data->User.Sid);
	}

#ifdef _DEBUG
	std::wstring token_information::build_privileges(HANDLE token)
	{
		const token_info::class_ptr<TokenPrivileges> privileges_data(token_info::get_instance_ptr<TokenPrivileges>(token));

		const std::size_t privileges_count(static_cast<std::size_t>(privileges_data->PrivilegeCount));

		std::vector<std::wstring> privileges_names;
		privileges_names.resize(privileges_count);

		for (std::size_t index = 0; index < privileges_count; ++index)
		{
			std::wstring& privilege_name(privileges_names[index]);
			privilege_name.assign(L"<invalid-name>");

			DWORD str_length(static_cast<DWORD>(privilege_name.length()));
			const BOOL is_ok(::LookupPrivilegeNameW(nullptr, &privileges_data->Privileges[index].Luid, (&privilege_name.front()), &str_length));
				
			privilege_name.resize(static_cast<std::size_t>(str_length));
			
			if (!is_ok && !::LookupPrivilegeNameW(nullptr, &privileges_data->Privileges[index].Luid, (&privilege_name.front()), &str_length))
				privilege_name.assign(L"<invalid-name>");
		}

		return string_utils::join(std::move(privileges_names), L"|");
	}

	std::wstring token_information::build_elevated(HANDLE token)
	{
		std::wstring result((0 != token_info::get_instance<TokenElevation>(token).TokenIsElevated) ? L"yes" : L"no");

		switch (token_info::get_instance<TokenElevationType>(token))
		{
		case TokenElevationTypeDefault:
			result += L"-default";
			break;
		case TokenElevationTypeFull:
			result += L"-full";
			break;
		case TokenElevationTypeLimited:
			result += L"-limited";
			break;
		default:
			result += L"-unknown";
			break;
		}

		return result;
	}
#endif // _DEBUG

	token_information::token_information(const handle_holder& token)
		: token_information(token.get())
	{}

	token_information::token_information(HANDLE token)
		: _session_id(build_session_id(token))
		, _sid_str(build_sid(token))
#ifdef _DEBUG
		, _privileges(build_privileges(token))
		, _elevated(build_elevated(token))
#endif // _DEBUG
	{}

	token_information::token_information(const token_information & other)
		: _session_id(other._session_id)
		, _sid_str(other._sid_str)
#ifdef _DEBUG
		, _privileges(other._privileges)
		, _elevated(other._elevated)
#endif // _DEBUG
	{}

	token_information::token_information(token_information&& other) noexcept
		: _session_id(0)
		, _sid_str()
#ifdef _DEBUG
		, _privileges()
		, _elevated()
#endif // _DEBUG
	{
		swap(other);
	}

	token_information & token_information::operator = (const token_information & other)
	{
		token_information(other).swap(*this);
		return *this;
	}

	token_information & token_information::operator = (token_information && other)
	{
		token_information(std::move(other)).swap(*this);
		return *this;
	}

	void token_information::swap(token_information& other) noexcept
	{
		std::swap(this->_session_id, other._session_id);
		this->_sid_str.swap(other._sid_str);
#ifdef _DEBUG
		this->_privileges.swap(other._privileges);
		this->_elevated.swap(other._elevated);
#endif // _DEBUG
	}

	session_id_t token_information::get_session_id() const
	{
		return _session_id;
	}

	std::wstring token_information::get_sid_str() const
	{
		return _sid_str;
	}

#ifdef _DEBUG
	std::wstring token_information::get_privileges() const
	{
		return _privileges;
	}

	std::wstring token_information::get_elevated() const
	{
		return _elevated;
	}
#endif // _DEBUG

	PSID get_sid_from_str(const sid_string_t & sid_str)
	{
		LOG_STACK();

		if (sid_str.empty())
		{
			LOG_DEBUG() << "Empty SID string was passed.";
			return nullptr;
		}

		PSID result = nullptr;
		if (!ConvertStringSidToSidW(sid_str.c_str(), &result))
			LOG_AND_THROW(win32::exception::by_last_error("ConvertStringSidToSid"));

		if (result == nullptr)
			LOG_AND_THROW(application_exception(result_code::unexpected_result_was_returned_from_convertstringsidtosid));

		return result;
	}

	handle_holder get_process_token()
	{
		LOG_STACK();

		return get_process_token(GetCurrentProcess());
	}

	handle_holder open_process(process_id_t process_id, DWORD desired_access = PROCESS_QUERY_INFORMATION)
	{
		LOG_STACK();

		HANDLE process = OpenProcess(desired_access, FALSE, process_id);
		if (process == HANDLE{})
			LOG_AND_THROW(win32::exception::by_last_error("OpenProcess"));
		return handle_holder(process);
	}

	handle_holder get_process_token(HANDLE process, DWORD desired_access /*= TOKEN_QUERY*/)
	{
		LOG_STACK();

		auto token = HANDLE{};
		if (!OpenProcessToken(process, desired_access, &token))
			LOG_AND_THROW(win32::exception::by_last_error("OpenProcessToken"));

		return handle_holder(token);
	}

	handle_holder get_process_token(process_id_t process_id)
	{
		LOG_STACK();

		return get_process_token(open_process(process_id).get());
	}

	handle_holder get_current_thread_token(DWORD desired_access)
	{
		LOG_STACK();

		const auto current_thread = GetCurrentThread();
		auto token = HANDLE{};
		if (!OpenThreadToken(current_thread, desired_access, TRUE, &token))
		{
			auto last_error = win32::get_last_error_code();
			if (last_error == win32::win32_errc::error_no_token)
			{
				return handle_holder();
			}
			else
			{
				LOG_AND_THROW(::win32::exception::by_error(std::move(last_error), "OpenThreadToken"));
			}
		}

		return handle_holder(token);
	}

	handle_holder get_network_service_primary_token()
	{
		LOG_STACK();

		HANDLE htoken = {};
		const auto logon_user_result = LogonUser(L"NetworkService", L"NT AUTHORITY", nullptr, LOGON32_LOGON_SERVICE, LOGON32_PROVIDER_DEFAULT, &htoken);

		handle_holder result{ htoken };

		if (logon_user_result == 0)
			LOG_AND_THROW(win32::exception::by_last_error("LogonUser"));

		return result;
	}

	PSID get_current_process_user_sid()
	{
		LOG_STACK();

		const token_info::class_ptr<TokenUser> user_data(token_info::get_instance_ptr<TokenUser>(get_process_token().get()));

		details::check_sid_and_attributes(*user_data.get());

		const auto sid_len = GetLengthSid(user_data->User.Sid);
		const auto sid_buffer = LocalAlloc(LPTR, static_cast<SIZE_T>(sid_len));
		if (sid_buffer == 0)
			LOG_AND_THROW(win32::exception::by_last_error("LocalAlloc"));

		if (!CopySid(sid_len, sid_buffer, user_data->User.Sid))
			LOG_AND_THROW(win32::exception::by_last_error("CopySid"));

		return sid_buffer;
	}

	template<typename func_t>
	void process_current_process_token_information(const func_t& func)
	{
		LOG_STACK();

		auto token_handle_holder = get_process_token();
		auto token_info = security::token_information{token_handle_holder};

		func(token_info);
	}

	std::tuple<sid_string_t, session_id_t> get_current_process_sid_and_session_id()
	{
		LOG_STACK();

		auto sid_string = sid_string_t{};
		auto session_id = session_id_t{};

		auto result = std::wstring{};
		process_current_process_token_information([&](const security::token_information& info)
		{
			sid_string = info.get_sid_str();
			session_id = info.get_session_id();
		});

		return std::make_tuple(std::move(sid_string), std::move(session_id));
	}

	std::wstring get_current_process_sid_string()
	{
		return std::get<0>(get_current_process_sid_and_session_id());
	}

	session_id_t get_current_process_session_id()
	{
		return std::get<1>(get_current_process_sid_and_session_id());
	}

	const wchar_t process_name_is_not_available[] = L"n/a";

	std::wstring get_process_name(const process_id_t process_id)
	{
		LOG_STACK();

		HANDLE hprocess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id);
		if (hprocess)
		{
			handle_holder holder(hprocess);

			const auto buffer_size = DWORD{UNICODE_STRING_MAX_CHARS};
			auto buffer = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };

			ZeroMemory(buffer.get(), buffer_size*sizeof(wchar_t));

			const auto call_result = GetProcessImageFileNameW(hprocess, buffer.get(), buffer_size);
			if (call_result == 0)
			{
				LOG_DEBUG() << "GetProcessImageFileNameW failed. Last error: " << std::hex << std::showbase << GetLastError();
				return std::wstring{process_name_is_not_available};
			}

			const auto backslash_ptr = wcsrchr(buffer.get(), '\\');
			const auto process_image_name = backslash_ptr ? backslash_ptr + 1 : buffer.get();
			return std::wstring{process_image_name};
		}
		else
		{
			LOG_DEBUG() << "Failed to open process pid: " << process_id;
			return std::wstring{process_name_is_not_available};
		}
	}

	std::wstring get_module_file_name(HINSTANCE instance)
	{
		LOG_STACK();

		const auto buffer_size = DWORD{UNICODE_STRING_MAX_CHARS};
		auto path_buffer = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };

		const auto get_module_file_name_result = GetModuleFileNameW(instance, path_buffer.get(), buffer_size);
		if (get_module_file_name_result == 0)
			LOG_AND_THROW(win32::exception::by_last_error("GetModuleFileNameW"));

		return{path_buffer.get()};
	}

	std::wstring get_current_module_path()
	{
		auto module = HMODULE{};

		if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCWSTR) &get_current_module_path,
			&module))
		{
			LOG_AND_THROW(win32::exception::by_last_error("GetModuleHandleExW"));
		}

		return get_module_file_name(module);
	}

	std::tuple<user_name_t, domain_name_t> get_user_name_and_domain_name(const sid_string_t & sid_string)
	{
		LOG_STACK();

		PSID p_sid = get_sid_from_str(sid_string);
		utils::on_exit sid_exit_guard([p_sid]
		{
			if (p_sid == nullptr) return;
			if (LocalFree(p_sid) != nullptr)
			{
				LOG_DEBUG() << "LocalFree failed.";
			}
		});

		SID_NAME_USE sid_name_use;
		DWORD account_num_of_chars = 0;
		DWORD domain_num_of_chars = 0;

		DWORD lookup_account_sid_last_error = 0;
		if (LookupAccountSidW(nullptr, p_sid, nullptr, &account_num_of_chars, nullptr, &domain_num_of_chars, &sid_name_use))
		{
			LOG_DEBUG() << "LookupAccountSid returned unexpected result.";
		}
		else
			lookup_account_sid_last_error = GetLastError();

		std::wstring user_name;
		std::wstring domain_name;

		if (account_num_of_chars == 0 && domain_num_of_chars == 0)
		{
			auto converted_sid_string = convert_sid_to_string(p_sid);
			LOG_DEBUG() << "LookupAccountSid failed. SID = " << converted_sid_string << " error code: " << win32::make_error_code(static_cast<win32::win32_errc>(lookup_account_sid_last_error)).message().c_str();
			user_name = std::move(converted_sid_string);
		}
		else
		{
			std::unique_ptr<WCHAR[]> account_name_buffer;
			std::unique_ptr<WCHAR[]> domain_name_buffer;

			if (account_num_of_chars != 0)
			{
				account_name_buffer.reset(new WCHAR[static_cast<std::size_t>(account_num_of_chars)]);
			}

			if (domain_num_of_chars != 0)
			{
				domain_name_buffer.reset(new WCHAR[static_cast<std::size_t>(domain_num_of_chars)]);
			}

			if (!LookupAccountSidW(
				nullptr,
				p_sid,
				account_name_buffer.get(),
				&account_num_of_chars,
				domain_name_buffer.get(),
				&domain_num_of_chars,
				&sid_name_use))
			{
				LOG_AND_THROW(win32::exception::by_last_error("LookupAccountSid"));
			}

			if (account_name_buffer)
			{
				user_name = account_name_buffer.get();
			}

			if (domain_name_buffer)
			{
				domain_name = domain_name_buffer.get();
			}
		}

		return std::tuple < user_name_t, domain_name_t > { std::move(user_name), std::move(domain_name) };
	}

	std::vector<WELL_KNOWN_SID_TYPE> get_privileged_sids()
	{
		return{WinLocalSystemSid, WinLocalServiceSid, WinBuiltinAdministratorsSid};
	}

	std::vector<WELL_KNOWN_SID_TYPE> get_network_service_and_local_service_ipc_sid()
	{
		auto result = get_privileged_sids();
		result.push_back(WinNetworkServiceSid);
		return result;
	}

	bool check_token_membership_in_any_group(
		const handle_holder & token,
		const std::vector<WELL_KNOWN_SID_TYPE>& groups)
	{
		LOG_STACK();

		for (const auto & sid : groups)
		{
			BOOL is_member = FALSE;
			const auto call_result = CheckTokenMembership(token.get(), details::create_well_known_sid(sid).get(), &is_member);
			if (call_result == 0)
				LOG_AND_THROW(win32::exception::by_last_error("CheckTokenMembership"));

			if (is_member)
				return true;
		}

		return false;
	}

	bool check_current_thread_membership_in_any_group(const std::vector<WELL_KNOWN_SID_TYPE>& groups)
	{
		LOG_STACK();
		return check_token_membership_in_any_group(handle_holder(), groups);
	}

	bool check_process_membership_in_any_group(process_id_t pid, const std::vector<WELL_KNOWN_SID_TYPE>& groups)
	{
		LOG_STACK();

		auto process_token_holder = get_process_token(open_process(pid, PROCESS_QUERY_INFORMATION).get(), TOKEN_QUERY | TOKEN_DUPLICATE);
		auto token_duplicate = HANDLE{};

		if (!DuplicateToken(process_token_holder.get(), SecurityIdentification, &token_duplicate))
			LOG_AND_THROW(win32::exception::by_last_error("DuplicateToken"));

		const auto duplicated_token_holder = handle_holder{token_duplicate};
		return check_token_membership_in_any_group(duplicated_token_holder, groups);
	}

	bool check_current_thread_token_is_local_system()
	{
		LOG_STACK();
		return check_token_membership_in_any_group(handle_holder(), {WinLocalSystemSid});
	}

	bool check_token_service_or_admin(const handle_holder & token)
	{
		LOG_STACK();
		return check_token_membership_in_any_group(token, get_privileged_sids());
	}

	//without exceptions, true or false only
	bool check_current_process_token_is_admin()
	{
		LOG_STACK();
		auto token = HANDLE{};
		if (::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &token))
		{
			const auto token_holder = handle_holder{token};
			CAccessToken process_token;
			CTokenGroups token_groups;

			process_token.Attach(token_holder.get());
			process_token.GetGroups(&token_groups);
			process_token.Detach();

			return (token_groups.LookupSid(Sids::Admins(), 0) == TRUE);
		}
		else
			return false;
	}

	scoped_impersonation::scoped_impersonation()
		: scoped_impersonation(handle_holder())
	{ }

	scoped_impersonation::scoped_impersonation(const handle_holder & access_token)
	{
		if (access_token)
		{
			try
			{
				const token_information info(access_token);
				m_user_sid = info.get_sid_str();
				LOG_TRACE()
					<< L"Impersonating logged on user"
					<< L" [sid: " << m_user_sid << L"]"
					<< L" [session: " << info.get_session_id() << L"]"
#ifdef _DEBUG
					<< L" [elevated: " << info.get_elevated() << L"]"
					<< L" [privileges: " << info.get_privileges() << L"]"
#endif // _DEBUG
					;
			}
			catch (...)
			{
				LOG_TRACE() << L"Impersonating logged on user [sid: <unknown>] [session: <unknown>]";
			}

			if (!ImpersonateLoggedOnUser(access_token.get()))
				LOG_AND_THROW(win32::exception::by_last_error("ImpersonateLoggedOnUser"));
		}
		else
		{
			m_user_sid = L"<unknown>";

			LOG_TRACE() << L"Impersonating logged [security impersonation]";

			if (!ImpersonateSelf(SecurityImpersonation))
				LOG_AND_THROW(win32::exception::by_last_error("ImpersonateSelf"));
		}
	}

	scoped_impersonation::~scoped_impersonation()
	{
		if (!::RevertToSelf())
		{
			LOG_DEBUG() << win32::exception::by_last_error("RevertToSelf").wwhat();
			return;
		}

		LOG_DEBUG() << L"Revert impersonating logged";
	}

	std::wstring scoped_impersonation::user_sid() const
	{
		return m_user_sid;
	}

	void try_execute_impersonated(
		const handle_holder & impersonate_token,
		const execute_for_impersonated_token_func_t & execute_for_impersonated_token_func,
		const execute_for_self_token_func_t & execute_for_self_token_func)
	{
		try_execute_impersonated_ex(
			impersonate_token,
			std::bind(execute_for_impersonated_token_func),
			(execute_for_self_token_func ? [&](const std::exception_ptr&) { execute_for_self_token_func(); } : execute_for_self_token_func_ex_t()));
	}

	void try_execute_impersonated_ex(
		const handle_holder & impersonate_token,
		const execute_for_impersonated_token_func_ex_t & execute_for_impersonated_token_func,
		const execute_for_self_token_func_ex_t & execute_for_self_token_func)
	{
		LOG_STACK();

		if (!execute_for_impersonated_token_func)
			LOG_AND_THROW(std::invalid_argument("execute_for_impersonated_token_func is not defined."));

		std::exception_ptr except_ptr;
		std::unique_ptr<security::scoped_impersonation> guard_impersonation;

		try
		{
			guard_impersonation = std::make_unique<security::scoped_impersonation>(impersonate_token);
		}
		catch (...)
		{
			if (!execute_for_self_token_func)
				throw;

			except_ptr = std::current_exception();
		}

		if (except_ptr)
		{
			LOG_DEBUG() << L"Execute alternative code for self token...";
			execute_for_self_token_func(except_ptr);
		}
		else
		{
			execute_for_impersonated_token_func(guard_impersonation->user_sid());
		}
	}

	LUID get_luid_from_token(const handle_holder & token)
	{
		LOG_STACK();

		const token_info::class_ptr<TokenAccessInformation> access_data(token_info::get_instance_ptr<TokenAccessInformation>(token.get()));

		LOG_TRACE() << "LUID.HighPart = " << access_data->AuthenticationId.HighPart << " , LUID.LowPart = " << access_data->AuthenticationId.LowPart;

		return access_data->AuthenticationId;
	}
}
