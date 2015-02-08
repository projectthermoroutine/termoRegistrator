/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 07.10.2014
// Краткое описание: реализация для алгоритма хеширования SHA-256
/////////////////////////////////////

#include <common/sha256.h>
#include <common/sha2.h>

namespace hash
{

	hash_t sha256(std::size_t block_size, const get_data_block_t& get_data_block)
	{
		if (block_size == 0U)
			throw std::invalid_argument("The passed argument block_size must be positive");

		if (!get_data_block)
			throw std::invalid_argument("The passed argument get_data_block is null");

		sha256_ctx context;
		sha256_begin(&context);

		std::vector<std::uint8_t> data_block(block_size);
		bool data_remains = false;
		do 
		{
			std::size_t data_written = 0;
			auto* data_ptr = data_block.data();
			data_remains = get_data_block(data_ptr, block_size, data_written);

			if (data_written)
				sha256_hash(data_ptr, data_written, &context);

		} while (data_remains);

		hash_t result(256 / 8);
		sha256_end(result.data(), &context);

		return result;
	}

	sha256_hash_t create_sha256()
	{
		hash_t result;
		sha256_ctx context;
		sha256_begin(&context);
		bool finished = false;

		return [=](const std::uint8_t* data_ptr, std::size_t data_size, bool last_block) mutable -> hash_t
		{
			if (finished)
				return result;

			if (data_size)
			{
				if (data_ptr == nullptr)
					throw std::invalid_argument("The passed argument data_ptr is null");
				sha256_hash(data_ptr, data_size, &context);
			}

			if (last_block)
			{
				result.resize(256 / 8);
				sha256_end(result.data(), &context);
				finished = true;
				return result;
			}
			else
				return hash_t();
		};
	}

	hash_t calculate_sha256(const std::uint8_t* data_ptr, std::size_t data_size)
	{
		if (data_ptr == nullptr && data_size != 0)
			throw std::invalid_argument("The passed argument data_ptr must be not is nullptr");

		sha256_ctx context;
		hash_t result(256 / 8);

		sha256_begin(&context);
		if (data_size)
			sha256_hash(data_ptr, data_size, &context);
		else
			if (data_ptr == nullptr)
				throw std::invalid_argument("The passed argument data_ptr is null");

		sha256_end(result.data(), &context);
		return result;
	}

}