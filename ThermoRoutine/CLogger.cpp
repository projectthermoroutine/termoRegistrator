#include "stdafx.h"
#include "CLogger.h"

#include <comutil.h>
#include <memory>

#include <loglib\log.h>
#include <common\string_utils.h>
#include <common\unhandled_exception.h>

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
	//	log4cplus::threadCleanup();

	return S_OK;
}

STDMETHODIMP
CLogger::InitializeLogger(
BSTR log_config_data,
BSTR logs_path,
BSTR log_file_name
)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	auto log_config_data_w = string_utils::convert_utf8_to_wchar(std::unique_ptr<char>(_com_util::ConvertBSTRToString(log_config_data)).get());
	auto logs_path_w = string_utils::convert_utf8_to_wchar(std::unique_ptr<char>(_com_util::ConvertBSTRToString(logs_path)).get());
	auto log_file_name_w = string_utils::convert_utf8_to_wchar(std::unique_ptr<char>(_com_util::ConvertBSTRToString(log_file_name)).get());

	unhandled_exception_handler::initialize(logs_path_w, [](const std::wstring & message) { LOG_FATAL() << message; });

	logger::initialize(log_config_data_w, logs_path_w, log_file_name_w, false, [](bool)	{});

	return S_OK;

}

