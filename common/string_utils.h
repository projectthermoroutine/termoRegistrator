#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <exception>
#include <numeric>

namespace string_utils
{
    std::wstring convert_utf8_to_wchar(const std::string& str);
    std::string convert_wchar_to_utf8(const std::wstring& str);

	std::string trim(const std::string &s);
	std::wstring trim(const std::wstring &s);

	template <typename T>
	std::string to_hex(T value)
	{
		std::stringstream ss;
		ss << std::hex << std::showbase << value;
		return ss.str();
	}

	class StringConversionException
		: public std::runtime_error
	{
	public:
		// Creates exception with error message and error code.
		StringConversionException(const char* message, unsigned long error)
			: std::runtime_error(message)
			, m_error(error)
		{}

		// Creates exception with error message and error code.
		StringConversionException(const std::string& message, unsigned long error)
			: std::runtime_error(message)
			, m_error(error)
		{}

		// Windows error code.
		unsigned long Error() const
		{
			return m_error;
		}

	private:
		unsigned long m_error;
	};


	std::wstring ConvertToUTF16(const std::string & source, const unsigned int codePage);


	template<typename char_t> inline typename std::size_t raw_str_length(const char_t*);
	template<> inline std::size_t raw_str_length<std::string::value_type>(const std::string::value_type* raw_str) { return std::strlen(raw_str); }
	template<> inline std::size_t raw_str_length<std::wstring::value_type>(const std::wstring::value_type* raw_str) { return std::wcslen(raw_str); }

	template<class string_t, template<class item_t, class allocator_t = std::allocator<item_t>> class container_t>
	string_t join(const container_t<string_t>& items, typename string_t::value_type const * delimiter = nullptr, bool skip_empty_items = false)
	{
		string_t result;

		if (!items.empty())
		{
			result = items.front();

			const std::size_t items_size(items.size());

			if (items_size > 1)
			{
				using size_type = typename string_t::size_type;

				const size_type delimiter_length(delimiter ? raw_str_length(delimiter) : 0);

				auto it(std::begin(items));
				const auto it_end(std::end(items));

				result.reserve(
					std::accumulate(
					it,
					it_end,
					size_type((items_size - 1) * delimiter_length),
					[](size_type length, const string_t& item)
				{
					return length + item.length();
				}));

				while (++it != it_end)
				{
					if (skip_empty_items && it->empty())
						continue;

					if (delimiter)
						result += delimiter;

					result += *it;
				}

				if (skip_empty_items && delimiter_length > 0 && items.front().empty())
					result.erase(0, delimiter_length);
			}
		}

		return result;
	}
	template<class string_t, template<class item_t, class compare_t = std::less<item_t>, class allocator_t = std::allocator<item_t>> class container_t>
	inline string_t join(const container_t<string_t>& items, typename string_t::value_type const * delimiter = nullptr, bool skip_empty_items = false)
	{
		std::vector<string_t> tmp_items(items.size());
		std::copy(std::begin(items), std::end(items), std::begin(tmp_items));
		return string_utils::join(tmp_items, delimiter, skip_empty_items);
	}
	template<class string_t>
	inline string_t join(const std::initializer_list<string_t>& items, typename string_t::value_type const * delimiter = nullptr, bool skip_empty_items = false)
	{
		return string_utils::join(std::vector<string_t>{items}, delimiter, skip_empty_items);
	}


}