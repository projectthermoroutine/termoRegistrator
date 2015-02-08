/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 29.09.2014
// Краткое описание: юнит-тест на модуль сериализации scanning_operations::clean_mode
/////////////////////////////////////

#include "CppUnitTest.h"
#include <unit_tests_common.h>
#include <common\scanning_operations_serialization_helpers.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

template<>
static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString(const scanning_operations::clean_mode& level)
{

	switch (level)
	{
	case scanning_operations::clean_mode::none:		return(L"none");
	case scanning_operations::clean_mode::standard:	return(L"standard");
	case scanning_operations::clean_mode::strict:	return(L"strict");
	case scanning_operations::clean_mode::rigorous:	return(L"rigorous");
	case scanning_operations::clean_mode::remove:	return(L"remove");
	default:
		throw std::logic_error("Unknown clean mode");
	};
};

namespace common_lib_tests_project
{
	TEST_CLASS(scanning_operations_serialization_helpers_test)
	{
	public:
		TEST_METHOD(test_read_from_stream)
		{
			checked_execute([] {				
				std::wistringstream stream(L"none standard strict rigorous remove wrong");

				scanning_operations::clean_mode none_mode, standard_mode, strict_mode, rigorous_mode, remove_mode;
				stream >> none_mode >> standard_mode >> strict_mode >> rigorous_mode >> remove_mode;

				Assert::AreEqual(scanning_operations::clean_mode::none, none_mode);
				Assert::AreEqual(scanning_operations::clean_mode::standard, standard_mode);
				Assert::AreEqual(scanning_operations::clean_mode::strict, strict_mode);
				Assert::AreEqual(scanning_operations::clean_mode::rigorous, rigorous_mode);
				Assert::AreEqual(scanning_operations::clean_mode::remove, remove_mode);
				
				Assert::ExpectException<const std::invalid_argument&>([&]{ scanning_operations::clean_mode wrong_mode; stream >> wrong_mode; }, L"It's possible to deserialize unknown mode");
			});
		}

		TEST_METHOD(test_write_to_stream)
		{
			checked_execute([] {
				std::wostringstream stream;
				
				stream << scanning_operations::clean_mode::none << " " << scanning_operations::clean_mode::standard << " " << scanning_operations::clean_mode::strict << " " << scanning_operations::clean_mode::rigorous << " " << scanning_operations::clean_mode::remove;

				Assert::AreEqual(std::wstring(L"none standard strict rigorous remove"), stream.str());
			});
		}
	};
}