/////////////////////////////////////
// (C) 2014 ООО "Код Безопасности"
// Проект: SNES-AV eset
// Автор: Зайцев Роман
// Создан: 26.09.2014
// Краткое описание: 
/////////////////////////////////////

#pragma once

#include <common/serialization.h>

namespace serialization
{
	template<typename T, typename U>
	struct serialization_friend : public T
	{
		serialization_friend(const T& src) : base_class(T){}
		template<typename... Args>	serialization_friend(Args&&... args) : T(std::forward<Args>(args)...){}

		friend binary_stream& operator<<(serialization::binary_stream& stream, const U& t)
		{
			stream << static_cast<const T&>(t);
			return stream;
		}

		friend binary_stream& operator>>(serialization::binary_stream& stream, U& t)
		{
			stream >> static_cast<T&>(t);
			return stream;
		}
	};

	template<typename T>
	struct serialization_helper : public serialization_friend<T, serialization_helper<T>>
	{
		using base_class = serialization_friend<T, serialization_helper>;
		serialization_helper(const base_class& src) : base_class(src){}
		template<typename... Args>	serialization_helper(Args&&... args) : base_class(std::forward<Args>(args)...){}
	};
}
