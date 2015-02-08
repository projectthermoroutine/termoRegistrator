/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 15.10.2014
// Краткое описание: Тесты на path_helpers
/////////////////////////////////////

#include "CppUnitTest.h"
#include <common\path_helpers.h>
#include <unit_tests_common.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{

	TEST_CLASS(path_helpers_test)
	{
	public:

		TEST_METHOD(test_vectors)
		{
			checked_execute([] {

				std::wstring wo_ext, ext;
				path_helpers::extract_file_path_and_extension(L"C:\\asd\\document.doc", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"C:\\asd\\document" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"doc" }, ext);

				path_helpers::extract_file_path_and_extension(L"C:\\asd\\document.", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"C:\\asd\\document" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);

				path_helpers::extract_file_path_and_extension(L"C:\\asd\\document.doc.zip", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"C:\\asd\\document.doc" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"zip" }, ext);

				path_helpers::extract_file_path_and_extension(L"C:\\asd\\document.doc.", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"C:\\asd\\document.doc" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);

				path_helpers::extract_file_path_and_extension(L"C:\\as.d\\document", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"C:\\as.d\\document" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);

				path_helpers::extract_file_path_and_extension(L"C:\\as.d\\document.txt", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"C:\\as.d\\document" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"txt" }, ext);

				path_helpers::extract_file_path_and_extension(L"", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);

				path_helpers::extract_file_path_and_extension(L".", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);

				path_helpers::extract_file_path_and_extension(L"a.", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"a" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);

				path_helpers::extract_file_path_and_extension(L".a", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"a" }, ext);

				path_helpers::extract_file_path_and_extension(L"..\\", wo_ext, ext);
				Assert::AreEqual(std::wstring{ L"..\\" }, wo_ext);
				Assert::AreEqual(std::wstring{ L"" }, ext);
			});
		}

	};
}