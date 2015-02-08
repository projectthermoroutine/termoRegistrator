/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 09.10.2014
// Краткое описание: юнит-тесты для модуля sha256
/////////////////////////////////////

#include "CppUnitTest.h"
#include <common\sha256.h>
#include <unit_tests_common.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace unit_tests_common;

namespace common_lib_tests_project
{
	
	TEST_CLASS(sha256_test)
	{
	public:

		static void run_test(const size_t block_size)
		{
			using hash_result = std::vector < std::uint8_t > ;
			using test_case = std::pair < const char*, hash_result > ;

			const size_t hash_size = 256 / 8;

			const test_case nist_tests[] =
			{
				{
					"abc",
					{ 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea, 0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23, 0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c, 0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad }
				},
				{
					"",
					{ 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 },
				},
				{
					"abcdbcde""cdefdefg""efghfghi""ghijhijk""ijkljklm""klmnlmno""mnopnopq",
					{ 0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8, 0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39, 0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67, 0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1 },
				},
				{
					"abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
					{ 0xcf, 0x5b, 0x16, 0xa7, 0x78, 0xaf, 0x83, 0x80, 0x03, 0x6c, 0xe5, 0x9e, 0x7b, 0x04, 0x92, 0x37, 0x0b, 0x24, 0x9b, 0x11, 0xe8, 0xf0, 0x7a, 0x51, 0xaf, 0xac, 0x45, 0x03, 0x7a, 0xfe, 0xe9, 0xd1 }
				}
			};

			for (size_t i = 0; i < sizeof(nist_tests) / sizeof(nist_tests[0]); ++i)
			{
				std::size_t index = 0;
				const auto data_length = strlen(nist_tests[i].first);
				const auto data_ptr = nist_tests[i].first;

				const auto pull_result = hash::sha256(block_size, [&](std::uint8_t* out_ptr, std::size_t block_size, std::size_t& written) -> bool
				{
					const auto test_length = static_cast<size_t>(data_length);
					if (index != test_length)
					{
						std::size_t j;
						for (j = 0; j < std::min(block_size, test_length-index); ++j)
							out_ptr[j] = data_ptr[index + j];
						
						written = j;
						index += written;
						return true;
					}
					else
						return false;
				});

				const auto hash_function = hash::create_sha256();				
				for (std::size_t k = 0; k != data_length;)
				{
					const auto current_block_size = std::min(block_size, data_length - k);
					hash_function(reinterpret_cast<const std::uint8_t*>(data_ptr)+k, current_block_size, false);
					k += current_block_size;
				}
				const auto push_result = hash_function(nullptr, 0, true);

				const auto single_call_result = hash::calculate_sha256(reinterpret_cast<const std::uint8_t*>(data_ptr), data_length);

				Assert::AreEqual(hash_size, pull_result.size());

				const auto correct_hash = nist_tests[i].second;
				for (size_t k = 0; k != hash_size; ++k)
				{
					Assert::AreEqual(correct_hash[k], pull_result[k]);
					Assert::AreEqual(correct_hash[k], push_result[k]);
					Assert::AreEqual(correct_hash[k], single_call_result[k]);
				}
			}
		}

		TEST_METHOD(test_vectors)
		{
			checked_execute([] {
				
				Assert::ExpectException<const std::invalid_argument&>([] {	run_test(0); });
				run_test(1);
				run_test(10);
				run_test(1000);

			});
		}

	};
}