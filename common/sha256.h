/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 07.10.2014
// Краткое описание: интерфейс для алгоритма хеширования SHA-256
/////////////////////////////////////

#pragma once

#include <vector>
#include <cstdint>
#include <functional>

namespace hash
{

	using hash_t = std::vector < std::uint8_t >;

	using get_data_block_t = std::function < bool(std::uint8_t*, std::size_t, std::size_t&) > ;
	hash_t sha256(std::size_t block_size, const get_data_block_t&);

	using sha256_hash_t = std::function < hash_t(const std::uint8_t*, std::size_t, bool) >;
	sha256_hash_t create_sha256();

	hash_t calculate_sha256(const std::uint8_t* data_ptr, std::size_t data_size);

}