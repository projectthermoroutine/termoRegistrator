#include "CppUnitTest.h"
#include <unit_tests_common.h>
#include <unhandled_exception.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{		
	TEST_CLASS(unhandled_exception_test)
	{
	public:		
		TEST_METHOD(initialization_test)
		{
			checked_execute([] {
				unhandled_exception_handler::initialize(L".", [](const std::wstring & message) {});
			});
		}
	};
}