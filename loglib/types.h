
#pragma once


#include <string>
#include <codecvt>
#include <cstdint>
#include <functional>


namespace logger
{

    enum class level : std::uint32_t
    {
        /* unknown = -1 */
        
        trace = 0,
        debug = 1,
        info  = 2,
        warn  = 3,
        error = 4,
        fatal = 5,
    };

	extern const level level_unknown;

    using use_developer_log_changed_f = std::function < void(bool) > ;

    struct settings
    {
        logger::level level;
        bool use_developer_log;
        std::uint64_t max_buffer_size;

        std::uint64_t max_file_size;
        std::size_t max_backup_index;


        using zip_t = std::tuple <
            decltype(logger::settings::level),
            decltype(logger::settings::use_developer_log),
            decltype(logger::settings::max_buffer_size),
            decltype(logger::settings::max_file_size),
            decltype(logger::settings::max_backup_index)
        > ;

        bool operator==(const settings& other) const;
        bool operator!=(const settings& other) const;

        zip_t zip() const;
        static settings unzip(const zip_t& zip);

        void validate(const std::function<void(const std::wstring & param_name, const std::wstring & original_value, const std::wstring & modified_value)>& invalid_notify);
    };

    class scope_logger;

    class log_stream;

    std::wstring log_level_to_string(level sev);
    level log_level_from_string(const std::wstring& str_sev);


} // namespace logger
