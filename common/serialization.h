#pragma once

#include <cstdint>
#include <string>
#include <deque>
#include <iterator>
#include <type_traits>
#include <stdexcept>

#include <common/tuple_helpers.h>

namespace serialization
{
	namespace details
	{
		using elements_num_t = std::uint64_t;
		using type_info_t = std::uint8_t;

		const type_info_t type_info_array = 0x80u;
		const type_info_t type_info_signed = 0x40u;
		const type_info_t type_info_bool = 0x20u;
		const type_info_t type_info_complex = 0x10u;

		template <typename iter_t>
		inline std::size_t insert_binary_data(iter_t & iter, std::uint8_t value)
		{
			*iter = value;
			++iter;

			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t insert_binary_data(iter_t & iter, std::uint16_t value)
		{
			*iter = static_cast<std::uint8_t>(value & 0xffu);
			++iter;
			value >>= 8u;
			*iter = static_cast<std::uint8_t>(value & 0xffu);
			++iter;

			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t insert_binary_data(iter_t & iter, std::uint32_t value)
		{
			*iter = static_cast<std::uint8_t>(value & 0xffu);
			++iter;
			value >>= 8u;
			*iter = static_cast<std::uint8_t>(value & 0xffu);
			++iter;
			value >>= 8u;
			*iter = static_cast<std::uint8_t>(value & 0xffu);
			++iter;
			value >>= 8u;
			*iter = static_cast<std::uint8_t>(value & 0xffu);
			++iter;

			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t insert_binary_data(iter_t & iter, std::uint64_t value)
		{
			insert_binary_data(iter, static_cast<std::uint32_t>(value & 0xfffffffful));
			insert_binary_data(iter, static_cast<std::uint32_t>(value >> 32u));
			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t insert_binary_data(iter_t & iter, bool value)
		{
			return insert_binary_data(iter, static_cast<std::uint8_t>(value ? 0xffu : 0u));
		}

		template <typename T>
		struct nearest_fixed_type :
			std::conditional
			<
				std::is_signed<T>::value,
				typename std::conditional < (sizeof(T) == sizeof(std::int8_t)), std::int8_t,
				typename std::conditional < (sizeof(T) == sizeof(std::int16_t)), std::int16_t,
				typename std::conditional < (sizeof(T) == sizeof(std::int32_t)), std::int32_t,
				typename std::conditional < (sizeof(T) == sizeof(std::int64_t)), std::int64_t, void>::type>::type>::type>::type,
				typename std::conditional < (sizeof(T) == sizeof(std::uint8_t)), std::uint8_t,
				typename std::conditional < (sizeof(T) == sizeof(std::uint16_t)), std::uint16_t,
				typename std::conditional < (sizeof(T) == sizeof(std::uint32_t)), std::uint32_t,
				typename std::conditional < (sizeof(T) == sizeof(std::uint64_t)), std::uint64_t, void>::type>::type>::type>::type
			>
		{};

		template <typename iter_t>
		inline std::size_t insert_binary_data(iter_t & iter, wchar_t value)
		{
			return insert_binary_data(iter, static_cast<nearest_fixed_type<wchar_t>::type>(value));
		}

		template <typename iter_t, typename value_t>
		typename std::enable_if<std::is_signed<value_t>::value, size_t>::type
		insert_binary_data(iter_t & iter, value_t value)
		{
			return insert_binary_data(
				iter,
				static_cast<std::make_unsigned<value_t>::type>(value));
		}

		template <typename iter_t, typename value_t>
		typename std::enable_if<std::is_enum<value_t>::value, size_t>::type
			insert_binary_data(iter_t & iter, value_t value)
		{
				return insert_binary_data(
					iter,
					static_cast<std::underlying_type<value_t>::type>(value));
		}

		template <typename iter_t>
		inline std::size_t get_binary_data(iter_t && iter, std::uint8_t & value)
		{
			value = *iter;
			++iter;

			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t get_binary_data(iter_t && iter, std::uint16_t & value)
		{
			value = *iter;
			++iter;
			value |= (*iter) << 8u;
			++iter;

			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t get_binary_data(iter_t && iter, std::uint32_t & value)
		{
			value = *iter;
			++iter;
			value |= (*iter) << 8u;
			++iter;
			value |= (*iter) << 16u;
			++iter;
			value |= (*iter) << 24u;
			++iter;

			return sizeof(value);
		}

		template <typename iter_t>
		inline std::size_t get_binary_data(iter_t && iter, std::uint64_t & value)
		{
			std::uint32_t low, high;
			auto read = get_binary_data(iter, low);
			read += get_binary_data(iter, high);

			value = (static_cast<std::uint64_t>(high) << 32u) | low;

			return read;
		}

		template <typename iter_t>
		inline std::size_t get_binary_data(iter_t && iter, bool & value)
		{
			std::uint8_t raw_value;
			auto const read = get_binary_data(iter, raw_value);
			value = (raw_value != 0u);
			return read;
		}

		template <typename iter_t>
		inline std::size_t get_binary_data(iter_t && iter, wchar_t & value)
		{
			nearest_fixed_type<wchar_t>::type raw_value;
			auto const read = get_binary_data(iter, raw_value);
			value = static_cast<wchar_t>(raw_value);
			return read;
		}

		template <typename iter_t, typename value_t>
		typename std::enable_if<std::is_signed<value_t>::value, size_t>::type
		get_binary_data(iter_t && iter, value_t & value)
		{
			std::make_unsigned<value_t>::type tmp;
			auto const read = get_binary_data(iter, tmp);
			value = static_cast<value_t>(tmp);
			return read;
		}

		template <typename iter_t, typename value_t>
		typename std::enable_if<std::is_enum<value_t>::value, size_t>::type
		get_binary_data(iter_t && iter, value_t & value)
		{
			std::underlying_type<value_t>::type tmp;
			auto const read = get_binary_data(iter, tmp);
			value = static_cast<value_t>(tmp);
			return read;
		}

		template <typename T, bool array_type>
		inline typename std::enable_if< std::is_enum<T>::value, type_info_t >::type get_type_info()
		{
			return get_type_info<typename std::underlying_type<T>::type, array_type>();
		}

		template <typename T, bool array_type>
		inline typename std::enable_if< !(std::is_integral<T>::value || std::is_enum<T>::value), type_info_t >::type get_type_info()
		{
			return type_info_complex | (array_type ? type_info_array : 0);
		}

		template <typename T, bool array_type>
		inline typename std::enable_if< (std::is_integral<T>::value && !std::is_same<T, bool>::value), type_info_t >::type get_type_info()
		{
			return sizeof(T) | (array_type ? type_info_array : 0) | (std::is_signed<T>::value ? type_info_signed : 0);
		}

		template <typename T, bool array_type>
		inline typename std::enable_if< (std::is_integral<T>::value && std::is_same<T, bool>::value), type_info_t >::type get_type_info()
		{
			return type_info_bool | (array_type ? type_info_array : 0);
		}

	}

	class deserialization_type_error : public std::exception
	{
		const char * what() const override
		{
			return "Unexpected type of value.";
		}
	};

	class deserialization_partial_buffer_error : public std::exception
	{
		const char * what() const override
		{
			return "The buffer for deserialization contains only a part of required data.";
		}
	};

	class deserialization_empty_buffer_error : public deserialization_partial_buffer_error
	{
		const char * what() const override
		{
			return "The buffer for deserialization is empty.";
		}
	};

	class binary_stream
	{
	private:
		using container_t = std::deque<std::uint8_t>;
	public:
		using const_iterator = container_t::const_iterator;
		using insert_iterator = std::back_insert_iterator<container_t>;

		binary_stream() : _read_pos(0)
		{
		}

		template <typename T>
		binary_stream & operator << (T value)
		{			
			auto const type_info = details::get_type_info<T, false>();

			auto iter = std::back_inserter(_buffer);
			details::insert_binary_data(iter, type_info);
			details::insert_binary_data(iter, value);
			
			return *this;
		}

		template <typename T>
		binary_stream & operator >> (T & value)
		{			
			check_partial_buffer_error(sizeof(details::type_info_t));

			auto const expected_type_info = details::get_type_info<T, false>();
			details::type_info_t actual_type_info;
			_read_pos += details::get_binary_data(_buffer.begin() + _read_pos, actual_type_info);
			if (actual_type_info != expected_type_info)
			{
				throw deserialization_type_error();
			}

			check_partial_buffer_error(sizeof(value));
			_read_pos += details::get_binary_data(_buffer.begin() + _read_pos, value);
			return *this;
		}

		template <typename value_t, typename iterator_t>
		binary_stream & write_array(iterator_t begin, iterator_t end)
		{
			auto const type_info = details::get_type_info<value_t, true>();
			const details::elements_num_t num_of_elements = static_cast<details::elements_num_t>(std::distance(begin, end));

			auto write_iter = std::back_inserter(_buffer);
			details::insert_binary_data(write_iter, type_info);
			details::insert_binary_data(write_iter, num_of_elements);

			write_content_of_array<value_t>(begin, end);

			return *this;
		}

		template <typename value_t, typename iterator_t>
		binary_stream & read_array(iterator_t dest)
		{
			check_partial_buffer_error(sizeof(details::type_info_t));

			auto const expected_type_info = details::get_type_info<value_t, true>();

			details::type_info_t actual_type_info;
			_read_pos += details::get_binary_data(_buffer.begin() + _read_pos, actual_type_info);
			if (actual_type_info != expected_type_info)
			{
				throw deserialization_type_error();
			}

			check_partial_buffer_error(sizeof(details::elements_num_t));

			details::elements_num_t num_of_elements = 0;
			_read_pos += details::get_binary_data(_buffer.begin() + _read_pos, num_of_elements);

			read_content_of_array<value_t>(num_of_elements, dest);

			return *this;
		}

		inline std::size_t buffer_size() const
		{
			return _buffer.size();
		}

		inline const_iterator cbegin() const
		{
			return _buffer.cbegin();
		}

		inline const_iterator cend() const
		{
			return _buffer.cend();
		}

		inline insert_iterator insert_iter()
		{
			return std::back_inserter(_buffer);
		}
	private:
		template <typename value_t, typename iterator_t>
		typename std::enable_if<std::is_integral<value_t>::value, void>::type
		write_content_of_array(iterator_t begin, iterator_t end)
		{
			auto write_iter = std::back_inserter(_buffer);
			for (auto iter = begin; iter != end; ++iter)
			{
				details::insert_binary_data(write_iter, *iter);
			}
		}

		template <typename value_t, typename iterator_t>
		typename std::enable_if<!(std::is_integral<value_t>::value), void >::type
		write_content_of_array(iterator_t begin, iterator_t end)
		{
			for (auto iter = begin; iter != end; ++iter)
			{
				*this << *iter;
			}
		}

		template <typename value_t, typename iterator_t>
		typename std::enable_if<std::is_integral<value_t>::value, void >::type
		read_content_of_array(details::elements_num_t num_of_elements, iterator_t dest)
		{
			check_partial_buffer_error(sizeof(value_t)* num_of_elements);

			for (decltype(num_of_elements) n = 0; n != num_of_elements; ++n, ++dest)
			{
				value_t value;
				_read_pos += details::get_binary_data(_buffer.begin() + _read_pos, value);
				*dest = value;
			}
		}

		template <typename value_t, typename iterator_t>
		typename std::enable_if<!(std::is_integral<value_t>::value), void >::type
		read_content_of_array(details::elements_num_t num_of_elements, iterator_t dest)
		{
			for (decltype(num_of_elements) n = 0; n != num_of_elements; ++n, ++dest)
			{
				value_t tmp;
				*this >> tmp;
				*dest = tmp;
			}
		}

		template <typename T>
		void check_partial_buffer_error(T to_read) const
		{
			if (_buffer.size() < (_read_pos + to_read))
			{
				_buffer.empty() ?
					throw deserialization_empty_buffer_error() :
					throw deserialization_partial_buffer_error();
			}
		}

		std::size_t _read_pos;
		container_t _buffer;
	};

	template <typename T>
	binary_stream & operator << (binary_stream & out, const std::basic_string<T> & str)
	{
		auto const length = str.length();
		return out.write_array<std::basic_string<T>::value_type>(str.data(), str.data() + length);
	}

	template <typename T>
	binary_stream & operator >> (binary_stream & in, std::basic_string<T> & str)
	{
		std::basic_string<T> result;
		in.read_array<std::basic_string<T>::value_type>(std::back_inserter(result));
		result.swap(str);

		return in;
	}

	template <typename container_t>
	typename std::enable_if<std::is_class<container_t>::value, binary_stream &>::type
	operator << (binary_stream & out, const container_t & container)
	{
		return out.write_array<typename container_t::value_type>(container.begin(), container.end());
	}

	template <typename container_t>
	typename std::enable_if<std::is_class<container_t>::value, binary_stream &>::type
	operator >> (binary_stream & in, container_t & container)
	{
		container_t result;
		in.read_array<typename container_t::value_type>(std::back_inserter(result));
		result.swap(container);

		return in;
	}

	namespace details
	{
		class serializer final
		{
		public:
			serializer(serialization::binary_stream & out) : _out(out) {}

			template <typename T>
			inline void operator () (const T & val)
			{
				_out << val;
			}

			serializer(const serializer &) = delete;
			serializer & operator = (const serializer &) = delete;
		private:
			serialization::binary_stream & _out;
		};

		template <typename tuple_t>
		void serialize_tuple(tuple_t tuple, serialization::binary_stream & out)
		{
			details::serializer serializer(out);
			tuple_helpers::for_each_in_tuple(tuple, serializer);
		}

		template <typename tuple_t, std::size_t N>
		typename std::tuple_element<N, tuple_t>::type deserialize_element(serialization::binary_stream & in)
		{
			using type = typename std::tuple_element<N, tuple_t>::type;
			typename type value;
			in >> value;
			return value;
		}

		template <typename tuple_t, std::size_t... n>
		tuple_t deserialize_tuple_impl(serialization::binary_stream & in, tuple_helpers::seq<n...>)
		{
			return tuple_helpers::revert_tuple(std::make_tuple(
				deserialize_element<tuple_t,
				(std::tuple_size<tuple_t>::value - n - 1)>(in)...)); // reverted order due calling convention
		}

		template <typename tuple_t, size_t N>
		tuple_t deserialize_tuple_impl(serialization::binary_stream & in, tuple_helpers::depth_hint<N>)
		{
			return deserialize_tuple_impl<tuple_t>(in, tuple_helpers::make_seq<N>());
		}

		template <typename tuple_t>
		std::tuple<> deserialize_tuple_impl(serialization::binary_stream & , tuple_helpers::depth_hint<0>)
		{
			return std::tuple<>();
		}

		template <typename tuple_t>
		tuple_t deserialize_tuple(serialization::binary_stream & in)
		{
			return details::deserialize_tuple_impl<tuple_t>(
				in,
				tuple_helpers::depth_hint<std::tuple_size<tuple_t>::value>());
		}
	}

	template <typename... types>
	binary_stream & operator << (binary_stream & out, const std::tuple<types...> & x)
	{
		details::serialize_tuple(x, out);
		return out;
	}

	template <typename... types>
	binary_stream & operator >> (binary_stream & in, std::tuple<types...> & x)
	{
		x = details::deserialize_tuple< std::tuple<types...> >(in);
		return in;
	}
}