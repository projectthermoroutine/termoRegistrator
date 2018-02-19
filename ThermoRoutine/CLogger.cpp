#include "stdafx.h"
#include "CLogger.h"

#include <comutil.h>
#include <memory>

#include <loglib\log.h>
#include <common\string_utils.h>
#include <common\unhandled_exception.h>
#include <common\path_helpers.h>

CLogger::CLogger()
{
}

CLogger::~CLogger()
{
	Close();
}

STDMETHODIMP
CLogger::Close()
{
	logger::deinitialize();

	return S_OK;
}

STDMETHODIMP
CLogger::InitializeLogger(
BSTR log_config_full_filename,
BSTR logs_path,
BSTR log_file_name
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto log_config_full_filename_w = string_utils::convert_utf8_to_wchar(std::unique_ptr<char>(_com_util::ConvertBSTRToString(log_config_full_filename)).get());

	const auto config_path = path_helpers::get_directory_path(log_config_full_filename_w);
	const auto config_filename = path_helpers::get_file_name(log_config_full_filename_w);

	auto logs_path_w = string_utils::convert_utf8_to_wchar(std::unique_ptr<char>(_com_util::ConvertBSTRToString(logs_path)).get());
	auto log_file_name_w = string_utils::convert_utf8_to_wchar(std::unique_ptr<char>(_com_util::ConvertBSTRToString(log_file_name)).get());

	unhandled_exception_handler::initialize(logs_path_w, [](const std::wstring & message) { LOG_FATAL() << message; });

	logger::initialize(config_path, config_filename, logs_path_w, log_file_name_w, true, [](bool)	{});

	return S_OK;

}

