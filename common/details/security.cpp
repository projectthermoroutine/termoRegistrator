#include <security.h>
#include <cstdlib>
#include <stdexcept>
#include <memory>
#include <functional>
#include <loglib\log.h>

#include <Windows.h>
#include <Aclapi.h>
#include <AccCtrl.h>

namespace security
{
	namespace details
	{
		using sid_ptr = std::unique_ptr<SID, decltype(&LocalFree)>;
		using acl_ptr = std::unique_ptr<ACL, decltype(&LocalFree)>;
		using sd_ptr = std::unique_ptr<SECURITY_DESCRIPTOR, decltype(&LocalFree)>;
		using func_uses_sd_t = std::function<void(PSECURITY_DESCRIPTOR)>;

		details::sid_ptr initialize_access_entry(
			EXPLICIT_ACCESSW & ea,
			WELL_KNOWN_SID_TYPE sid_type,
			DWORD access_permissions)
		{
			LOG_STACK();

			DWORD sid_size = SECURITY_MAX_SID_SIZE;
			sid_ptr p_sid(
				reinterpret_cast<SID*>(LocalAlloc(LMEM_FIXED, sid_size)),
				&LocalFree);

			if (p_sid == nullptr)
			{
				const auto last_error = GetLastError();
				LOG_DEBUG() << "LocalAlloc failed: " << std::hex << std::showbase << last_error;
				throw std::runtime_error("Could not allocate memory for SID");
			}

			if (!CreateWellKnownSid(sid_type, nullptr, p_sid.get(), &sid_size))
			{
				const auto last_error = GetLastError();
				LOG_DEBUG() << "CreateWellKnownSid failed: " << std::hex << std::showbase << last_error;
				throw std::runtime_error("Could not create well known SID");
			}

			ea.grfInheritance = NO_INHERITANCE;
			ea.grfAccessMode = SET_ACCESS;
			ea.grfAccessPermissions = access_permissions;
			ea.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
			ea.Trustee.pMultipleTrustee = nullptr;
			ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea.Trustee.ptstrName = reinterpret_cast<decltype(ea.Trustee.ptstrName)>(p_sid.get());

			return p_sid;
		}

		void process_sd_for_internal_ipc(func_uses_sd_t func_uses_sd, DWORD access_permissions)
		{
			LOG_STACK();

			using namespace details;

			const std::size_t number_of_entries = 3;
			EXPLICIT_ACCESSW explicit_access[number_of_entries];
			SecureZeroMemory(&explicit_access, sizeof(decltype(explicit_access)));

			const auto sid_local_system = initialize_access_entry(
				explicit_access[0],
				WinLocalSystemSid,
				access_permissions);

			const auto sid_local_service = initialize_access_entry(
				explicit_access[1],
				WinLocalServiceSid,
				access_permissions);

			const auto sid_local_admins = initialize_access_entry(
				explicit_access[2],
				WinBuiltinAdministratorsSid,
				access_permissions);

			acl_ptr p_acl(nullptr, &LocalFree);

			{
				PACL p_acl_raw = nullptr;
				const auto result_code = SetEntriesInAclW(
					number_of_entries,
					explicit_access,
					nullptr,
					&p_acl_raw);

				if (result_code != ERROR_SUCCESS)
				{
					LOG_DEBUG() << "SetEntriesInAclW failed: " << std::hex << std::showbase << result_code;
					throw std::runtime_error("Could not set ACL");
				}
				p_acl.reset(p_acl_raw);
			}

			sd_ptr p_sd(
				reinterpret_cast<SECURITY_DESCRIPTOR*>(LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH)),
				&LocalFree);

			if (!InitializeSecurityDescriptor(p_sd.get(), SECURITY_DESCRIPTOR_REVISION))
			{
				const auto last_error = GetLastError();
				LOG_DEBUG() << "InitializeSecurityDescriptor failed: " << std::hex << std::showbase << last_error;
				throw std::runtime_error("Could not initialize security descriptor");
			}

			if (!SetSecurityDescriptorDacl(p_sd.get(), TRUE, p_acl.get(), FALSE))
			{
				const auto last_error = GetLastError();
				LOG_DEBUG() << "SetSecurityDescriptorDacl failed: " << std::hex << std::showbase << last_error;
				throw std::runtime_error("Could not set security descriptor's DACL");
			}

			func_uses_sd(p_sd.get());
		}
	} // namespace details

	void process_sec_attributes_for_internal_ipc(func_uses_sec_attr_t func_uses_sec_attr, DWORD access_permissions)
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

		details::process_sd_for_internal_ipc(process_func, access_permissions);
	}

	void process_sec_attributes_for_internal_pipe(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		return process_sec_attributes_for_internal_ipc(func_uses_sec_attr, GENERIC_READ | GENERIC_WRITE);
	}

	void process_sec_attributes_for_internal_event_sync(func_uses_sec_attr_t func_uses_sec_attr)
	{
		LOG_STACK();
		return process_sec_attributes_for_internal_ipc(func_uses_sec_attr, SYNCHRONIZE);
	}

}