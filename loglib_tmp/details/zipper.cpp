#include <Windows.h>
#include <tuple>
#include <memory>
#include <zlib.h>
#include <cstdint>
#include <fstream>
#include <functional>
#include <common\path_helpers.h>
#include "zipper.h"

namespace zip
{
	namespace
	{
		class on_exit final
		{
		public:
			using func_t = std::function<void(void)>;
			on_exit(func_t func) : _func(func), _cancelled(false) {}

			~on_exit()
			{
				if (_cancelled) return;

				try
				{
					_func();
				}
				catch (const std::exception&)
				{
					std::abort();
				}
				catch (...)
				{
					std::abort();
				}

			}

			void cancel()
			{
				_cancelled = true;
			}

			on_exit(const on_exit &) = delete;
			on_exit & operator = (const on_exit &) = delete;
		private:
			func_t _func;
			bool _cancelled;
		};

		bool file_exists(const wstring_t &file_full_path)
		{
			if (file_full_path.empty())
			{
				return false;
			}

			DWORD attributes = GetFileAttributesW(file_full_path.c_str());

			return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		std::uint64_t get_file_size(const wstring_t &file_full_path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;

			if (!GetFileAttributesExW(file_full_path.c_str(), GetFileExInfoStandard, &fad))
			{
				throw std::runtime_error("GetFileAttributesExW failed in cpplogger::get_file_size");
			}

			LARGE_INTEGER file_size;
			file_size.LowPart = fad.nFileSizeLow;
			file_size.HighPart = fad.nFileSizeHigh;

			if (file_size.QuadPart < 0)
			{
				throw std::runtime_error("File size can't be below zero");
			}

			return file_size.QuadPart;
		}

		SYSTEMTIME get_file_system_datetime(const wstring_t &file_full_path)
		{
			WIN32_FILE_ATTRIBUTE_DATA fad;
			SYSTEMTIME sys_time;

			if (!GetFileAttributesExW(file_full_path.c_str(), GetFileExInfoStandard, &fad))
			{
				throw std::runtime_error("GetFileAttributesExW failed in cpplogger::get_file_system_datetime");
			}

			if (!FileTimeToSystemTime(&fad.ftLastWriteTime, &sys_time))
			{
				throw std::runtime_error("FileTimeToSystemTime failed in cpplogger::get_file_system_datetime");
			}

			return sys_time;
		}

		std::size_t get_preferred_compressed_data_size(std::size_t size)
		{
			const auto preferred_size = size + size * 0.0001 + 12;
			if (preferred_size > std::numeric_limits<std::size_t>::max())
			{
				throw std::out_of_range("compression_exception: Too big entry file size: " + std::to_string(size));
			}

			return static_cast<std::size_t>(preferred_size);
		}

		bool check_datetime_range(std::uint16_t value, std::uint16_t low, std::uint16_t high)
		{
			return (value >= low && value <= high) ? true : false;
		}

		std::uint16_t get_file_date_modification(const wstring_t &file_full_path)
		{
			const SYSTEMTIME datetime = get_file_system_datetime(file_full_path);
			std::uint16_t year;
			std::uint16_t month;
			std::uint16_t day;

			check_datetime_range(datetime.wYear, 1980, 2107) ? year = datetime.wYear - 1980 : throw std::out_of_range("Out of range 'year' value: " + std::to_string(datetime.wYear));
			check_datetime_range(datetime.wMonth, 1, 12) ? month = datetime.wMonth : throw std::out_of_range("Out of range 'month' value: " + std::to_string(datetime.wMonth));
			check_datetime_range(datetime.wDay, 1, 31) ? day = datetime.wDay : throw std::out_of_range("Out of range 'day' value: " + std::to_string(datetime.wDay));

			const std::uint16_t date = static_cast <std::uint16_t>(datetime.wDay + (datetime.wMonth << 5) + (year << 9));
			return date;
		}

		std::uint16_t get_file_time_modification(const wstring_t &file_full_path)
		{
			const SYSTEMTIME datetime = get_file_system_datetime(file_full_path);
			std::uint16_t hour;
			std::uint16_t minute;
			std::uint16_t second;

			check_datetime_range(datetime.wHour, 0, 24) ? hour = datetime.wHour : throw std::out_of_range("Out of range 'hour' value: " + std::to_string(datetime.wHour));
			check_datetime_range(datetime.wMinute, 0, 59) ? minute = datetime.wMinute : throw std::out_of_range("Out of range 'minute' value: " + std::to_string(datetime.wMinute));
			check_datetime_range(datetime.wSecond, 0, 59) ? second = datetime.wSecond : throw std::out_of_range("Out of range 'second' value: " + std::to_string(datetime.wSecond));

			const std::uint16_t time = static_cast <std::uint16_t>((datetime.wSecond / 2) + (datetime.wMinute << 5) + (datetime.wHour << 11));
			return time;
		}

		std::tuple <wstring_t, wstring_t, wstring_t, wstring_t> split_path(wstring_t file_full_path)
		{
			const auto long_path_prefix = std::wstring{ L"\\\\?\\" };
			const bool is_long_path = file_full_path.compare(0, long_path_prefix.size(), long_path_prefix) == 0;

			if (is_long_path)
				file_full_path.erase(std::begin(file_full_path), std::begin(file_full_path) + long_path_prefix.size());

			const auto buffer_size = std::size_t{ 32768 };

			auto drive_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(drive_name_buf.get(), buffer_size*sizeof(drive_name_buf[0]));
			auto dir_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(dir_name_buf.get(), buffer_size*sizeof(dir_name_buf[0]));
			auto file_name_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(file_name_buf.get(), buffer_size*sizeof(file_name_buf[0]));
			auto extension_buf = std::unique_ptr < wchar_t[] > { new wchar_t[static_cast<std::size_t>(buffer_size)] };
			SecureZeroMemory(extension_buf.get(), buffer_size*sizeof(extension_buf[0]));

			auto const split_result = _wsplitpath_s(
				file_full_path.c_str(),
				drive_name_buf.get(),
				buffer_size,
				dir_name_buf.get(),
				buffer_size,
				file_name_buf.get(),
				buffer_size,
				extension_buf.get(),
				buffer_size);

			if (split_result != 0)
				throw std::runtime_error("Could not split path: " + string_utils::convert_wchar_to_utf8(file_full_path));

			auto drive_name = std::wstring{ drive_name_buf.get() };
			auto dir_name = std::wstring{ dir_name_buf.get() };
			auto file_name = std::wstring{ file_name_buf.get() };
			auto file_ext = std::wstring{ extension_buf.get() };

			if (drive_name.empty() && dir_name.empty() && file_name.empty() && file_ext.empty())
				throw std::runtime_error("Could not split path: " + string_utils::convert_wchar_to_utf8(file_full_path) + ";result is empty");

			if (is_long_path && !drive_name.empty())
				drive_name = long_path_prefix + drive_name;

			return std::make_tuple(std::move(drive_name), std::move(dir_name), std::move(file_name), std::move(file_ext));
		}

		wstring_t get_file_name(const wstring_t &file_full_path)
		{
			auto file_name = wstring_t();
			auto file_ext = wstring_t();
			std::tie(std::ignore, std::ignore, file_name, file_ext) = split_path(file_full_path);

			return file_name + file_ext;
		}
	}

	namespace details
	{
		auto const local_file_header_signature = 0x04034b50;
		auto const central_directory_signature = 0x02014b50;
		auto const end_central_directory_signature = 0x06054b50;
		auto const archivation_flag_mask = 0xC02;
		auto const zip_compression_method = Z_DEFLATED;
		auto const max_compressed_file_size = UINT32_MAX;
		auto const max_zip_entries_count = UINT16_MAX;
		auto const deflate_attempt_count = 20;

		/*Zip document file service headers*/
#pragma pack(push,1)

		struct local_file_header
		{
			std::uint32_t signature;
			std::uint16_t version_to_extract;
			std::uint16_t general_purpose_bit_flag;
			std::uint16_t compression_method;
			std::uint16_t modification_time;
			std::uint16_t modification_date;
			std::uint32_t crc32;
			std::uint32_t compressed_size;
			std::uint32_t uncompressed_size;
			std::uint16_t file_name_length;
			std::uint16_t extra_field_length;
		};

		struct central_directory_file_header
		{
			std::uint32_t signature;
			std::uint16_t version_made_by;
			std::uint16_t version_to_extract;
			std::uint16_t general_purpose_bit_flag;
			std::uint16_t compression_method;
			std::uint16_t modification_time;
			std::uint16_t modification_date;
			std::uint32_t crc32;
			std::uint32_t compressed_size;
			std::uint32_t uncompressed_size;
			std::uint16_t file_name_length;
			std::uint16_t extra_field_length;
			std::uint16_t file_comment_length;
			std::uint16_t disk_number;
			std::uint16_t internal_file_attr;
			std::uint32_t external_file_attr;
			std::uint32_t local_file_header_offset;
		};

		struct end_central_directory_record
		{
			std::uint32_t signature;
			std::uint16_t disk_number;
			std::uint16_t start_disk_number;
			std::uint16_t number_central_directory_record;
			std::uint16_t total_central_directory_record;
			std::uint32_t size_central_directory;
			std::uint32_t central_directory_offset;
			std::uint16_t comment_length;
		};

#pragma pack(pop)

		struct zip_entry
		{
			local_file_header file_header_;
			central_directory_file_header central_directory_;
			wstring_t file_full_path_;
			wstring_t file_name_;
		};

		/*compressor class implementation*/
		class compressor final
		{
		public:
			compressor(const wstring_t &zip_out_file_path, const wstring_t &zip_out_file_name, compression_level_t compression_level)
				: compression_level_(compression_level)
			{
				if (compression_level_ != Z_DEFAULT_COMPRESSION && (compression_level_ < 0 || compression_level_ > 9))
				{
					throw compression_exception(L"Invalid compression level value: " + std::to_wstring(compression_level) + L", must equal -1 or exists in the range 0..9");
				}

				if (zip_out_file_name.empty() || zip_out_file_path.empty())
				{
					throw compression_exception(L"Compressor out file paths are invalid: " + path_helpers::concatenate_paths(zip_out_file_path, zip_out_file_name));
				}

				zip_out_stream_.exceptions(std::ios_base::badbit | std::ios_base::failbit);
				zip_out_full_path_ = path_helpers::concatenate_paths(zip_out_file_path, zip_out_file_name);
			}

			void add_file(const wstring_t &file_full_path)
			{
				if (file_exists(file_full_path))
				{
					zip_entry entry;
					entry.file_full_path_ = file_full_path;
					entry.file_name_ = get_file_name(file_full_path);

					/*fill local file header*/
					SecureZeroMemory(&entry.file_header_, sizeof(entry.file_header_));
					entry.file_header_.signature = local_file_header_signature;
					entry.file_header_.general_purpose_bit_flag = entry.file_header_.general_purpose_bit_flag | archivation_flag_mask;
					entry.file_header_.compression_method = zip_compression_method;
					entry.file_header_.modification_date = get_file_date_modification(file_full_path);
					entry.file_header_.modification_time = get_file_time_modification(file_full_path);

					/*check the storage size for compressed data*/
					std::uint64_t file_size = get_file_size(file_full_path);
					if (static_cast<std::uint64_t>(file_size + file_size * 0.0001 + 12) > max_compressed_file_size)
					{
						throw compression_exception(L"compression_exception: Too big entry file: " + file_full_path + L", " + std::to_wstring(file_size));
					}

					entry.file_header_.uncompressed_size = static_cast<std::uint32_t>(file_size);
					entry.file_header_.file_name_length = static_cast<std::uint16_t>(string_utils::convert_wchar_to_utf8(entry.file_name_).size());

					if (entry.file_header_.file_name_length > MAX_PATH)
					{
						throw compression_exception(L"compression_exception: File name length is too big: " + std::to_wstring(entry.file_header_.file_name_length));
					}

					/*fill central directory file header*/
					SecureZeroMemory(&entry.central_directory_, sizeof(entry.central_directory_));
					entry.central_directory_.signature = central_directory_signature;
					entry.central_directory_.general_purpose_bit_flag = entry.file_header_.general_purpose_bit_flag;
					entry.central_directory_.compression_method = entry.file_header_.compression_method;
					entry.central_directory_.modification_time = entry.file_header_.modification_time;
					entry.central_directory_.modification_date = entry.file_header_.modification_date;
					entry.central_directory_.uncompressed_size = entry.file_header_.uncompressed_size;
					entry.central_directory_.file_name_length = entry.file_header_.file_name_length;

					zip_entries_.push_back(entry);
				}
				else
				{
					throw compression_exception(L"Entry file doesn't exist: " + file_full_path);
				}
			}

			void create_archive()
			{
				if (zip_entries_.size() > max_zip_entries_count)
				{
					throw compression_exception(L"compression_exception: Too much zip entry files: " + std::to_wstring(zip_entries_.size()));
				}

				zip_out_stream_.open(zip_out_full_path_, std::ios_base::binary | std::ios_base::out);

				if (zip_out_stream_.good())
				{
					write_file_header_and_data();
					write_central_directory();
					write_end_central_directory();
				}
			}

			compressor(const compressor &) = delete;
			compressor &operator=(const compressor &) = delete;

		private:
			void write_file_header_and_data()
			{
				for (auto it = zip_entries_.begin(); it != zip_entries_.end(); ++it)
				{
					std::vector <std::uint8_t> source_data_buffer;
					std::vector <std::uint8_t> destination_data_buffer;

					std::ifstream entry_file_stream(it->file_full_path_, std::ios_base::binary | std::ios_base::in);

					if (it->file_header_.uncompressed_size != 0)
					{
						source_data_buffer.resize(it->file_header_.uncompressed_size);
						destination_data_buffer.resize(get_preferred_compressed_data_size(source_data_buffer.size()));

						entry_file_stream.read(reinterpret_cast<char*>(source_data_buffer.data()), it->file_header_.uncompressed_size);

						if (source_data_buffer.size() > std::numeric_limits<uInt>::max())
						{
							throw compression_exception(L"Zipper compression exception: Too big source_data_buffer size");
						}

						it->file_header_.crc32 = ::crc32(0, source_data_buffer.data(), static_cast<uInt>(source_data_buffer.size()));
						it->central_directory_.crc32 = it->file_header_.crc32;
					}
					else
					{
						const auto null_crc = ::crc32(0L, Z_NULL, 0);
						it->file_header_.crc32 = null_crc;
						it->central_directory_.crc32 = null_crc;
					}

					/*Compression algorithms*/
					bool all_compressed = false;
					std::size_t deflate_counter = 0;

					z_stream zlib_stream{};

					on_exit deflate_exit_guard([&]
					{
						deflateEnd(&zlib_stream);
					});

					if (deflateInit2(&zlib_stream, compression_level_, zip_compression_method, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK)
					{
						throw compression_exception(L"Zlib initialization error.");
					}

					if (!source_data_buffer.empty())
					{
						zlib_stream.avail_in = static_cast<uInt>(source_data_buffer.size());
						zlib_stream.next_in = source_data_buffer.data();
						std::size_t data_written = 0;

						while (!all_compressed)
						{
							zlib_stream.avail_out = static_cast<uInt>(destination_data_buffer.size() - data_written);
							auto *out_ptr = destination_data_buffer.data() + data_written;
							zlib_stream.next_out = out_ptr;

							int result = deflate(&zlib_stream, Z_FINISH);

							if (result == Z_OK)
							{
								data_written += zlib_stream.next_out - out_ptr;
								destination_data_buffer.resize(get_preferred_compressed_data_size(destination_data_buffer.size()));

								if (deflate_counter == deflate_attempt_count)
								{
									throw compression_exception(L"Zlib compression error on file: " + it->file_name_);
								}
							}
							else if (result == Z_STREAM_END)
							{
								all_compressed = true;
							}
							else
							{
								break;
							}

							++deflate_counter;
						}
					}

					it->file_header_.compressed_size = zlib_stream.total_out;
					it->central_directory_.compressed_size = zlib_stream.total_out;

					std::fstream::pos_type position = zip_out_stream_.tellp();
					if (position < 0)
					{
						throw compression_exception(L"compression_exception: Negative size of output zip file");
					}

					it->central_directory_.local_file_header_offset = static_cast<std::uint32_t>(position);

					/*write local file header*/
					zip_out_stream_.write(reinterpret_cast<char*>(&it->file_header_), sizeof(it->file_header_));

					/*write file name*/
					zip_out_stream_.write(string_utils::convert_wchar_to_utf8(it->file_name_).c_str(), it->file_header_.file_name_length);

					/*write data*/
					if (!destination_data_buffer.empty())
					{
						zip_out_stream_.write(reinterpret_cast<char*>(destination_data_buffer.data()), it->file_header_.compressed_size);
					}
				}

				std::fstream::pos_type position = zip_out_stream_.tellp();
				if (position < 0)
				{
					throw compression_exception(L"compression_exception: Negative size of output zip file");
				}

				if (position > max_compressed_file_size)
				{
					throw compression_exception(L"compression_exception: Too big size of output zip file");
				}

				first_offset_cdfh_ = static_cast<std::uint32_t>(position);
			}

			void write_central_directory()
			{
				for (auto entry : zip_entries_)
				{
					zip_out_stream_.write(reinterpret_cast<char*>(&entry.central_directory_), sizeof(entry.central_directory_));
					zip_out_stream_.write(string_utils::convert_wchar_to_utf8(entry.file_name_).c_str(), entry.file_header_.file_name_length);
				}

				std::fstream::pos_type position = zip_out_stream_.tellp();
				if (position < 0)
				{
					throw compression_exception(L"compression_exception: Negative size of output zip file");
				}

				if (position > max_compressed_file_size)
				{
					throw compression_exception(L"compression_exception: Too big size of output zip file");
				}

				last_offset_cdfh_ = static_cast<std::uint32_t>(zip_out_stream_.tellp());
			}

			void write_end_central_directory()
			{
				end_central_directory_record eocd;
				SecureZeroMemory(&eocd, sizeof(eocd));
				eocd.central_directory_offset = first_offset_cdfh_;
				eocd.number_central_directory_record = static_cast<std::uint16_t>(zip_entries_.size());
				eocd.total_central_directory_record = eocd.number_central_directory_record;
				
				if (last_offset_cdfh_ < first_offset_cdfh_)
				{
					throw compression_exception(L"compression_exception: Negative zip central directory size");
				}

				eocd.size_central_directory = last_offset_cdfh_ - first_offset_cdfh_;

				eocd.signature = end_central_directory_signature;
				zip_out_stream_.write(reinterpret_cast<char*>(&eocd), sizeof(eocd));
			}

			std::ofstream zip_out_stream_;
			std::vector <zip_entry> zip_entries_;
			std::uint32_t first_offset_cdfh_;
			std::uint32_t last_offset_cdfh_;
			compression_level_t compression_level_;
			wstring_t zip_out_full_path_;
		};
	}//END details

	void create_archive(const std::vector<wstring_t> & file_names,
		const wstring_t &zip_out_file_path,
		const wstring_t &zip_out_file_name,
		compression_level_t compression_level)
	{
		details::compressor c(zip_out_file_path, zip_out_file_name, compression_level);

		for (const auto &full_file_name : file_names)
		{
			c.add_file(full_file_name);
		}

		c.create_archive();
	}
}//END zip