#pragma once

namespace utils
{
	inline uint64_t xor_shift_next(uint64_t x)
	{
		x ^= x >> 12;
		x ^= x << 25;
		x ^= x >> 27;
		return x * 2685821657736338717LL;
	}
}